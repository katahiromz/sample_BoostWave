encoding ライブラリ
========

概要
--------

A simple character code conversion library for VC++2012 or later.

C++標準ライブラリのみを用いて作られた、マルチバイト文字列、ワイド文字列、UTF-8文字列の相互変換ライブラリです。  
ワイド文字型がUTF-16であることを前提としてワイド文字列⇔UTF-8文字列の変換を実装しているため、VC++以外ではおそらくまともに使えないでしょう。  
加えて、C++11の機能やライブラリを利用しているため、VC++2012以降でなければ使えません。

ソースコードのライセンスは MIT License です。

導入手順
--------

このライブラリはヘッダのみで構成されています。  
ルートディレクトリにインクルードパスを通すことですぐに利用可能です。  
すべての定義は名前空間 `encoding` 以下に属しています。

サンプルをVC++でビルドするには、最新の cmake をインストールして、ルートディレクトリにある cmake_vs20XX_x86.bat を実行してください。  
ルートディレクトリ直下に _build_vs20XX_x86 というディレクトリが作られ、その中にVC++のソリューションファイルが配置されます。

使い方
--------

sample/sample.cpp を読めば大体わかります。

基本的には encoding/convert.hpp か encoding/convert_as.hpp をインクルードして、 `convert` 関数か `convert_as` 関数を使います。

```cpp
#include <encoding/convert.hpp>
#include <encoding/convert_as.hpp>
#include <vector>

int main(int, char**)
{
    // マルチバイト文字列をワイド文字列に変換
    const auto wide = encoding::convert("あいうえお", encoding::narrow_to_wide);

    // ワイド文字列をUTF-8文字列に変換して vector で受け取る
    const auto utf8 = encoding::convert_as<std::vector<char>>(wide, encoding::wide_to_utf8);

    // vector に入っているUTF-8文字列をマルチバイト文字列に変換
    const auto narr = encoding::convert(utf8, encoding::utf8_to_narrow);

    return 0;
}
```

いずれの関数も第1引数に変換元の文字列、第2引数に文字コード変換種別タグ変数（後述）を指定します。  
`convert_as` 関数ではそれに加えて第1テンプレート引数に変換先の型を指定します。

文字列には、 `std::string` 、 `std::vector<char>` 、 `const char*` などの型が利用できます。（`const char*` は引数に渡す場合のみ）  
ただしワイド文字列の場合はそれらの代わりに `std::wstring` 、 `std::vector<wchar_t>` 、 `const wchar_t*` などの型を使う必要があります。  
ワイド文字列を渡すべきところに `char` を使ったり、あるいはその逆の記述をしたりするとコンパイルエラーになります。

なお、もし必要であれば第3引数にロケール設定を渡すことができます。  
第3引数を省略した場合は `std::locale("")` が渡されます。  
ロケール設定はマルチバイト文字との変換処理にのみ用いられ、ワイド文字列⇔UTF-8文字列の変換処理では無視されます。

文字コード変換種別タグ
--------

現在、下記の static const 変数が定義されています。

|変数名          |変換元            |変換先            |
|----------------|------------------|------------------|
|`narrow_to_wide`|マルチバイト文字列|ワイド文字列      |
|`wide_to_narrow`|ワイド文字列      |マルチバイト文字列|
|`narrow_to_utf8`|マルチバイト文字列|UTF-8文字列       |
|`utf8_to_narrow`|UTF-8文字列       |マルチバイト文字列|
|`wide_to_utf8`  |ワイド文字列      |UTF-8文字列       |
|`utf8_to_wide`  |UTF-8文字列       |ワイド文字列      |
