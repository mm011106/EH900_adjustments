# EH900 adjustments

## EH-900の調整作業を行います

EH-900に使われている、電流源、ADコンバータ（2ch）のゼロ調整（オフセット調整）とゲイン調整をして、ボード上のメモリに初期設定値として書き込みます。  
この作業はEH-900のファームウエアを書き込む前に必須です。

## 準備
- ダミー抵抗　113.74オーム
- DMM　直流電流測定モードに設定し、出力電流を測定するように接続
- 本機に繋ぐUSBシリアルのコンソール（9600bps),　UBSケーブル

## 手順
1. オフセット調整
    - ADコンバータの入力をショートし、電圧を計測することでADコンバータのオフセットを測定し、その値を記録します。

2. 電流出力調整と電流読み値の値付け

3. 電圧測定部利得調整

4. パラメタの確認と書き込み

## コマンド
o: オフセット調整  
    - ADコンバータのオフセットを調整（測定）します。
    - 準備ができてたら`g + リターン`で実行。

c: 電流出力調整
    - 電流源の出力電流を調整します。
    - 流れている電流を計測できるように外部のDMMを接続します。
        - これがこの装置の電流の値付けになります。
    - 初期設定値で電流を出力し、自分で測定した値を表示します。
    - 75mAに近くなるように`[u|d] + リターン`で調整します。
    - 調整完了したらDMMの電流値を読み取って、`q+ リターン`
    - 電流値の入力を求められるので、入力します。
    - 計算された系数などが表示されるので確認します。

g: 電圧測定部利得調整
    - 接続されている抵抗（センサを模擬するタミー抵抗）の値113.74オームと電流値を基準に、電圧測定の利得誤差をキャリブレーションします。
    - 準備ができてたら`g + リターン`で実行。

w: パラメタ書き込み
    - 調整値の確認をして、よければそのままメモリに書き込むことができます。
    - もし値がおかしければ、中断(q)して調整をやり直します
    - 値の確認ができてたら`g + リターン`で書き込みます。

## 手順
順番は大切です。コマンドの o, c, g はこの順番に行ってください。  

