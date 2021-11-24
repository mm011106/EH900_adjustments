# EH900 adjustments

## EH-900の調整作業を行います

EH-900に使われている、電流源、ADコンバータ（2ch）のゼロ調整（オフセット調整）とゲイン調整をして、ボード上のメモリに初期設定値として書き込みます。  
この作業はEH-900のファームウエアを書き込む前に必須です。

このバージョンからは外部クロックがデフォルトで有効になります。外部クロックの用意がない場合は `EH900_clkConfig.h` のincludeを無効にしてコンパイルすることで、HSI（内部クロック）を利用するように設定されます。

## 準備
- ダミー抵抗　113.74オーム
- DMM　直流電流測定モードに設定し、出力電流を測定するように接続
- 本機に繋ぐUSBシリアルのコンソール（115200bps),　UBSケーブル

## 手順
1. **(o)オフセット調整**
    - ADコンバータの入力をショートし、電圧を計測することでADコンバータのオフセットを測定し、その値を記録します。

2. **(c)電流出力調整と電流読み値の値付け**
    - 電流源の設定値を調整して、75mAにできるだけ近づけます。
    - さらに、DMMで測定した電流値を真値として、電流源と電流測定回路の値付けを行います。
  
3. **(g)電圧測定部利得調整**
    - 値付けされた電流源と電流測定回路に既知の抵抗を接続し、電流を流すことで発生する電圧（基準となる電圧）から電圧測定部の利得の値付けを行います。

4. **(V)VMON出力DACオフセット調整**
    - VMONに0.1Vを出力し、それを実測した値からオフセット(LSB)を求めます。VMON出力の確度を向上させます。
  
5. **(w)パラメタの確認と書き込み**
    - 測定した調整パラメタをFRAMに保存します。
    - 保存しないと液面計のソフトウエアが正しく動作しません。
  
実行の順番は大切です。o, c, g, v の順番に行ってください。  


## コマンド
**o: オフセット調整**  
- ADコンバータのオフセットを調整（測定）します。
- 準備ができてたら`g + リターン`で実行。

**c: 電流出力調整**
- 電流源の出力電流を調整します。
- 流れている電流を計測できるように外部にDMMを接続します。
- 初期設定値で電流が出力され、それを内部の回路で測定した値が表示されます。
- 表示された電流が75mAに近くなるように`[u|d] + リターン`で調整します。
- 調整完了したらDMMの電流値を読み取って、`q+ リターン`
- DMMで読み取った電流値の入力を求められるので入力します。
- その値から計算された補正系数などが表示されるので確認します。

**g: 電圧測定部利得調整**
- 接続されている抵抗（センサを模擬するタミー抵抗）の値**113.74オーム**と電流値を基準に、電圧測定の利得誤差をキャリブレーションします。
- 準備ができてたら`g + リターン`で実行。

**v: VMON出力DACオフセット調整**
- VMON出力端にDMM（電圧計測）接続します。
- 準備ができてたら`g + リターン`で実行。
- VMONに0.1Vが出力されるので、実測データを読み取って**ボルト単位**で入力します。
- 補正後の電圧が再度設定されるので、0.1Vに近づいていることを確認します。

**w: パラメタ書き込み**
- 調整値が表示されますので、内容を確認をしてください。
- もし値がおかしければ、中断(`q`)して調整をやり直します
- 値の確認ができてたら`g + リターン`で書き込みます。


