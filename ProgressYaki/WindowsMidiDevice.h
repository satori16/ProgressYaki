#pragma once
#include <chrono>
#include <memory>
#include <vector>
#include <Windows.h>
#include <mmsystem.h>

#pragma comment (lib, "winmm.lib")

//ñ≥ífÇ≈égÇ¡ÇΩÇÁégópóøÇPÇOÇOâ≠â~Ç¢ÇΩÇæÇ´Ç‹Ç∑ÅIÅIÅI

class MIDIDevice {
	typedef std::shared_ptr<HMIDIOUT> SharedMIDIOUT;
public:
	typedef std::shared_ptr<bool> RefBool;
	class MidiSender {
	public:
		bool Initialise(SharedMIDIOUT SMO, RefBool B) {
			H = SMO;
			Destroyed = B;
			return true;
		}
		bool IsDestroyed() { return *Destroyed; }
		bool Destroy() {
			if (IsDestroyed()) { return false; }
			H = std::make_shared<HMIDIOUT>();
			Destroyed = std::make_shared<bool>(true);
		}
		bool SendEvent(BYTE Command, BYTE Param1, BYTE Param2) {
			if (IsDestroyed()) { return false; }
			DWORD P = (Command | Param1 << 8 | Param2 << 16);
			MMRESULT R = midiOutShortMsg(*H, P);
			if (R != MMSYSERR_NOERROR) return false;

			return true;

		}
		bool SendLongData( LPSTR Data, DWORD Len) {
				if (IsDestroyed()) { return false; }

				MIDIHDR MH = { 0, };
				MH.lpData = Data;
				MH.dwBufferLength = Len;

				MMRESULT R = midiOutPrepareHeader(*H, &MH, sizeof(MIDIHDR));
				if (R != MMSYSERR_NOERROR) return false;

				R = midiOutLongMsg(*H, &MH, sizeof(MIDIHDR));
				//if (R != MMSYSERR_NOERROR) return false;
				R = midiOutUnprepareHeader(*H, &MH, sizeof(MIDIHDR));
				if (R != MMSYSERR_NOERROR) return false;

				return true;
			}
		bool SendLongData(CHAR command,CHAR Param1, LPSTR Data, DWORD Len) {
				if (IsDestroyed()) { return false; }

				std::vector<CHAR> D;
				D.push_back(command);
				D.push_back(Param1);
				DWORD A = Len & 0x7f | (Len & 0x7f80) << 1 | (Len & 0x7f8000) << 2 | (Len & 0x7f800000) << 3;

				if (A & 0x800000)D.push_back((A >> 21) & 0x7f);
				if (A & 0x8000)D.push_back((A >> 14) & 0x7f);
				if (A & 0x80)D.push_back((A >> 7) & 0x7f);
				D.push_back(A & 0x7f);
				D.insert(D.end(), Data, Data + Len);

				return SendLongData(D.data(), static_cast<DWORD>(D.size()));
				/** /
				MIDIHDR MH = { 0, };
				MH.lpData = D.data();
				MH.dwBufferLength = static_cast<DWORD>(D.size());

				MMRESULT R = midiOutPrepareHeader(*H, &MH, sizeof(MIDIHDR));
				if (R != MMSYSERR_NOERROR) return false;

				R = midiOutLongMsg(*H, &MH, sizeof(MIDIHDR));
				//if (R != MMSYSERR_NOERROR) return false;
				R = midiOutUnprepareHeader(*H, &MH, sizeof(MIDIHDR));
				if (R != MMSYSERR_NOERROR) return false;

				return true;
				/**/
			}
		bool SendSysEX(CHAR command, LPSTR Data, DWORD Len) {
				if (IsDestroyed()) { return false; }

				std::vector<CHAR> D;
				D.push_back(command);
				DWORD A = Len & 0x7f | (Len & 0x7f80) << 1 | (Len & 0x7f8000) << 2 | (Len & 0x7f800000) << 3;

				if (A & 0x800000)D.push_back((A >> 21) & 0x7f);
				if (A & 0x8000)D.push_back((A >> 14) & 0x7f);
				if (A & 0x80)D.push_back((A >> 7) & 0x7f);
				D.push_back(A & 0x7f);
				D.insert(D.end(), Data, Data + Len);
				
				return SendLongData(D.data(), static_cast<DWORD>(D.size()));
				/** /
				MIDIHDR MH = { 0, };
				MH.lpData = D.data();
				MH.dwBufferLength = D.size();


				MMRESULT R = midiOutPrepareHeader(*H, &MH, sizeof(MIDIHDR));
				if (R != MMSYSERR_NOERROR) return false;

				R = midiOutLongMsg(*H, &MH, sizeof(MIDIHDR));
				//if (R != MMSYSERR_NOERROR) return false;
				R = midiOutUnprepareHeader(*H, &MH, sizeof(MIDIHDR));
				if (R != MMSYSERR_NOERROR) return false;

				return true;
				/**/
			}
			bool DeviceReset() {
				return midiOutReset(*H) == MMSYSERR_NOERROR ? true : false;
			}
	protected:
		SharedMIDIOUT H;
		RefBool Destroyed;
	};
	typedef std::shared_ptr<MidiSender> SharedMidiSender;

	bool Initalize(UINT DeviceID) {
		if (IsCreated()) { return false; }
		MMRESULT R;
		R = midiOutOpen(&(*H), DeviceID, 0, 0, CALLBACK_NULL);
		if (R != MMSYSERR_NOERROR) { return false; }
		Created = true;
		IsVolumeChange = false;
		*Destroyed = false;
		return true;
	}
	bool IsCreated() { return Created; }

	bool Destroy() {
		if (!IsCreated()) { return false; }
		*Destroyed = true;
		if (IsVolumeChange) midiOutSetVolume(*H, DefaultVolume);
		midiOutReset(*H);
		midiOutClose(*H);
		return true;
	}
	bool SetVolume(DWORD V) {
		/**/
		if (!IsVolumeChange) {
			MMRESULT R = midiOutGetVolume(*H, &DefaultVolume);
			if (R != MMSYSERR_NOERROR) { return false; }
			IsVolumeChange = true;
		}
		/**/
		MMRESULT R = midiOutSetVolume(*H, V);
		if (R != MMSYSERR_NOERROR) { return false; }
		return true;
	}
	bool SendLongData(LPSTR Data, DWORD Len) {
		if (!IsCreated()) { return false; }

		MIDIHDR MH = { 0, };
		MH.lpData = Data;
		MH.dwBufferLength = Len;

		MMRESULT R = midiOutPrepareHeader(*H, &MH, sizeof(MIDIHDR));
		if (R != MMSYSERR_NOERROR) return false;

		R = midiOutLongMsg(*H, &MH, sizeof(MIDIHDR));
		//if (R != MMSYSERR_NOERROR) return false;
		R = midiOutUnprepareHeader(*H, &MH, sizeof(MIDIHDR));
		if (R != MMSYSERR_NOERROR) return false;

		return true;
	}
	bool SendLongData(CHAR command, CHAR Param1, LPSTR Data, DWORD Len) {
		if (!IsCreated()) { return false; }

		std::vector<CHAR> D;
		D.push_back(command);
		D.push_back(Param1);
		DWORD A = Len & 0x7f | (Len & 0x7f80) << 1 | (Len & 0x7f8000) << 2 | (Len & 0x7f800000) << 3;

		if (A & 0x800000)D.push_back((A >> 21) & 0x7f);
		if (A & 0x8000)D.push_back((A >> 14) & 0x7f);
		if (A & 0x80)D.push_back((A >> 7) & 0x7f);
		D.push_back(A & 0x7f);
		D.insert(D.end(), Data, Data + Len);

		SendLongData(D.data(), static_cast<DWORD>(D.size()));
		/** /
		MIDIHDR MH = { 0, };
		MH.lpData = D.data();
		MH.dwBufferLength = D.size();

		MMRESULT R = midiOutPrepareHeader(*H, &MH, sizeof(MIDIHDR));
		if (R != MMSYSERR_NOERROR) return false;

		R = midiOutLongMsg(*H, &MH, sizeof(MIDIHDR));
		//if (R != MMSYSERR_NOERROR) return false;
		R = midiOutUnprepareHeader(*H, &MH, sizeof(MIDIHDR));
		if (R != MMSYSERR_NOERROR) return false;

		return true;
		/**/
	}
	bool SendSysEX(CHAR command, LPSTR Data, DWORD Len) {
		if (!IsCreated()) { return false; }

		std::vector<CHAR> D;
		D.push_back(command);
		DWORD A = Len & 0x7f | (Len & 0x7f80) << 1 | (Len & 0x7f8000) << 2 | (Len & 0x7f800000) << 3;

		if (A & 0x800000)D.push_back((A >> 21) & 0x7f);
		if (A & 0x8000)D.push_back((A >> 14) & 0x7f);
		if (A & 0x80)D.push_back((A >> 7) & 0x7f);
		D.push_back(A & 0x7f);
		D.insert(D.end(), Data, Data + Len);

		SendLongData(D.data(), static_cast<DWORD>(D.size()));
		/** /
		MIDIHDR MH = { 0, };
		MH.lpData = D.data();
		MH.dwBufferLength = D.size();

		MMRESULT R = midiOutPrepareHeader(*H, &MH, sizeof(MIDIHDR));
		if (R != MMSYSERR_NOERROR) return false;

		R = midiOutLongMsg(*H, &MH, sizeof(MIDIHDR));
		//if (R != MMSYSERR_NOERROR) return false;
		R = midiOutUnprepareHeader(*H, &MH, sizeof(MIDIHDR));
		if (R != MMSYSERR_NOERROR) return false;

		return true;
		/**/
	}
	bool SendGMReset() {
		CHAR M[] = {(CHAR)0xf0,(CHAR)0x7E, (CHAR)0x7F,  (CHAR)0x9, (CHAR) 0x1,(CHAR)  0xF7 };
		if (!SendLongData(M, sizeof(M))) { return false; }
		return true;
	}
	bool SendGSReset() {
		CHAR M[] = {(CHAR)0xf0,(CHAR)0x41,(CHAR)0x20,(CHAR)0x42,(CHAR)0x12,(CHAR)0x40,(CHAR)0x00,(CHAR)0x7F,(CHAR)0x00,(CHAR)0x41,(CHAR)0xF7 };
		if (!SendLongData(M, sizeof(M))) { return false; }
		return true;
	}

	SharedMidiSender GetMidiSender() {
		SharedMidiSender SMS = std::make_shared<MidiSender>();
		SMS->Initialise(H, Destroyed);
		return SMS;
	}
	bool DeviceReset() {
		return midiOutReset(*H) == MMSYSERR_NOERROR ? true : false;
	}

	virtual ~MIDIDevice() {
		Destroy();
	}


protected:

	SharedMIDIOUT H = std::make_shared<HMIDIOUT>();
	bool Created = false;
	RefBool Destroyed = std::make_shared<bool>(true);
	DWORD DefaultVolume = 0;
	bool IsVolumeChange = false;
};

	std::vector<MIDIOUTCAPS> EnumMidiDevice() {
		std::vector<MIDIOUTCAPS> D;
		MIDIOUTCAPS  C = { 0, };

		MMRESULT R;
		for (UINT i = 0; i < midiOutGetNumDevs(); i++) {
			R = midiOutGetDevCaps(i, &C, sizeof(MIDIOUTCAPS));
			if (R == MMSYSERR_NOERROR)D.push_back(C);
		}
		return D;


	}

double MakeOneTick(std::int64_t Tempo, std::int64_t TimeBase) {
	return (60 / static_cast<double>(Tempo)) / static_cast<double>(TimeBase)*(1000*1000*1000);

}