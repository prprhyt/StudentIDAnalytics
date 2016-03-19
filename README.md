# StudentIDAnalytics  

##概要  
東海大の学生証から読み取った学生IDを集計、分析してグラフやランキングを表示etc…したりするWindowsアプリケーションです。  
WindowsPCにつないだPasoriから学生証の非保護領域に記録されている学生IDを読み出し、ローカルストレージに記録します。  
読み込んだ複数の学生IDを分析して得た統計情報をドーナツグラフで表示します。  
また、学生IDの記録回数ランキングを表示します。(未実装)

## 動作確認済みの環境
OS: Windows8.1(x64)  
felicaリーダー: Pasori(RC-S330)  
NFCポートソフトウェア(旧: FeliCaポートソフトウェア) ver5.4.2.10  
2015年度春学期発行の東海大学の学生証

## 開発環境  
OS: Windows8.1(x64)  
IDE: Microsoft Visual Studio Community 2015  
felicaリーダー: Pasori(RC-S330)  
NFCポートソフトウェア(旧: FeliCaポートソフトウェア) ver5.4.2.10  
2015年度春学期発行の東海大学の学生証  

## 使用しているライブラリについて  
このソフトウェアは非保護領域からのデータの読み出しの際に行われるPasoriとの通信に[felicalib 0.4.2](http://felicalib.tmurakam.org/)を使用しています。  

## ライセンスについて
使用したライブラリと自作部分でライセンスを分けています。それぞれのライセンスの表示についてはLICENSE.mdを参照してください。  
また、それぞれに適用されたライセンスについては以下の通りです。
##### felicalib
felicalib(felicalib.c,felicalib.h,felicaint.h) はBSD (3-Clause) License (BSD(3条項)ライセンス) が適用されています。  
また、BSDライセンスに基づきライブラリの部分のライセンス表示とライブラリの著作者表示をREADME.mdで行っています。  
felicalibのライセンスについては[felicalib wikiのライセンスの項](http://felicalib.tmurakam.org/license.html)を参照してください。  

##### その他の部分
GNU GPL(General Public License) v2を適用しています。
