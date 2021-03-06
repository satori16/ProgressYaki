// JsonHolder IV.cpp : このファイルには 'main' 関数が含まれています。プログラム実行の開始と終了がそこで行われます。
//

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <variant>
#include <memory>
#include <cstdint>
#include <cctype>
#include "FlatMap.h"


//https://dev.to/tmr232/that-overloaded-trick-overloading-lambdas-in-c17
template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
template<class... Ts> overloaded(Ts...)->overloaded<Ts...>;

class JsonItem {
public:
	typedef std::shared_ptr<JsonItem> Shared;
	typedef std::string JsonElement;
	typedef std::pair <std::string, std::string> JsonPair;
	typedef std::vector<JsonItem> JsonArray;
	typedef std::shared_ptr<JsonArray> SharedJsonArray;
	typedef FlatMap<std::string, JsonItem> JsonMap;
	typedef std::shared_ptr<JsonMap> SharedJsonMap;
	typedef std::variant<JsonElement, JsonArray, JsonMap,SharedJsonArray,SharedJsonMap> JsonType;
public:
	JsonItem() {}
	JsonItem( const JsonType& In) :I(In),F(true) {}
	JsonItem( const JsonElement& In) :I(In),F(true) {}
	JsonItem( const JsonArray& In) :I(In),F(true) {}
	JsonItem( const JsonMap& In) :I(In),F(true) {}
	JsonItem( const SharedJsonArray& In) :I(In),F(true) {}
	JsonItem( const SharedJsonMap& In) :I(In),F(true) {}

	JsonItem(const std::string& name, const JsonType& In) :I(In),Name(name) {}
	JsonItem(const std::string& name,const JsonElement& In) :I(In),Name(name) {}
	JsonItem(const std::string& name,const JsonArray& In) :I(In),Name(name) {}
	JsonItem(const std::string& name,const JsonMap& In) :I(In),Name(name) {}
	JsonItem(const std::string& name,const SharedJsonArray& In) :I(In),Name(name) {}
	JsonItem(const std::string& name,const SharedJsonMap& In) :I(In),Name(name) {}
	
	template<class T>
	T& Get() {
		return std::get<T>(I);
	}
	std::string GetName() {
		return Name;
	}
	bool SetName(const std::string& In) {
		I = In;
		F = In.empty();
		return true;
	}
	bool SetItem(const JsonItem& In) {
		I = In.I;
		Name = In.Name;
		return true;
	}
	bool SetItem(const JsonType& In) {
		I = In;
		return true;
	}
	operator const JsonItem&() {
		return I;
	}
	const JsonItem& operator =(const JsonItem& In) {
		I = In.I;
		return In;
	}
	template<class T>
	auto Interpret() {
		return T(std::get<JsonElement>(I));
	}
	template<class T>
	auto Interpret(const T& F) {
		return F(std::get<JsonElement>(I));
	}
	bool IsElement() {
		return I.index() == 0;
	}
	bool IsContainer() {
		return !IsElement();
	}
	std::string ToString(bool F=false,std::size_t N = 0) {
		std::string R;
		std::string Tab = F? MakeTab(N):"";

		R += std::visit(overloaded{
						[&](auto& O)->std::string { return "Not Supprt Type"; },
						[&](JsonElement& i)->std::string { return Tab + (F ? i : Name + ':' + i) + ','; },
						[&](JsonArray& i)->std::string {std::string T; for (auto&o : i)T += Tab + "[\n" + o.ToString(F,N + 1) + Tab + "\n],"; return T; },
						[&](JsonMap& i)->std::string {std::string T; for (auto&o : i)T += Tab + "{\n" + o.first + ':' + o.second.ToString() + Tab + "\n},"; return T; },
						[&](SharedJsonArray& i)->std::string {std::string T; for(auto&o:*i)T+=Tab+"[\n"+o.ToString(F,N+1)+Tab+"\n],"; return T; },
						[&](SharedJsonMap& i)->std::string { std::string T; for(auto&o:*i)T+=Tab+"{\n"+o.first+':'+o.second.ToString()+Tab+"\n},"; return T;  },
			}, I);

		return R;
	}
protected:
	std::string MakeTab(std::size_t N,std::string T="  ") {
		std::string R;
		for (std::size_t i = 0; i < N; i++)R += T;
		return R;
	}
protected:
	std::string Name;
	JsonType I;
	bool F = false;
};

std::intmax_t ToInteger(const JsonItem::JsonElement& In) {
	return std::stoll(In);
}
double ToFloat(const JsonItem::JsonElement& In) {
	return std::stod(In);
}
bool ToBoolean(const JsonItem::JsonElement& In) {
	auto T = In;
	std::transform(In.begin(), In.end(),T.begin(), std::tolower);
	std::size_t i = T.find("true");
	return i!=T.npos ? true:false;
}
class CToInteger {
public:
	std::intmax_t operator()(const JsonItem::JsonElement& In) const{
		return std::stoll(In);
	}
};
class CToFloat {
public:
	double operator()(const JsonItem::JsonElement& In) const{
	return std::stod(In);
	}
};
int main()
{
	//FlatMap<int,int> M;
	JsonItem::JsonElement E = "true";
	JsonItem J = E;

	auto A = J.Interpret(CToInteger());
	
    std::cout << A; 
	return 0;
}

// プログラムの実行: Ctrl + F5 または [デバッグ] > [デバッグなしで開始] メニュー
// プログラムのデバッグ: F5 または [デバッグ] > [デバッグの開始] メニュー

// 作業を開始するためのヒント: 
//    1. ソリューション エクスプローラー ウィンドウを使用してファイルを追加/管理します 
//   2. チーム エクスプローラー ウィンドウを使用してソース管理に接続します
//   3. 出力ウィンドウを使用して、ビルド出力とその他のメッセージを表示します
//   4. エラー一覧ウィンドウを使用してエラーを表示します
//   5. [プロジェクト] > [新しい項目の追加] と移動して新しいコード ファイルを作成するか、[プロジェクト] > [既存の項目の追加] と移動して既存のコード ファイルをプロジェクトに追加します
//   6. 後ほどこのプロジェクトを再び開く場合、[ファイル] > [開く] > [プロジェクト] と移動して .sln ファイルを選択します
