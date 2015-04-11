# MikuMikuOpenWorld　＊名前は仮です<br>
このプロジェクトはMMDなどの素材を使いオープンソースで広大な世界を作ろうっていう企画です。<br>
現在は街を探索したり、チャットしたり、yotube/ニコ動の動画を見れるくらいしか機能がありませんが、<br>
将来的には色々なゲームがジャンルを問わず集合するようなゲームにしたいと思っています。<br>
<br>
・操作方法<br>
キャラ : W前進 S後進 A左に旋回 D右に旋回 Speceジャンプ 左Shift + W前進ダッシュ<br>
車 : 上キーアクセル 下キーブレーキ 左右キーハンドル Bブレーキ<br>
<br>
・動画再生にはmp4に対応したコーデックが必要です（推奨:K-Lite Codec Pack）<br>
・動画の再生同期は時刻で取っています。PCの時刻が正確でないと同期がずれる、及び再生できない<br>
場合があります。サーバー側の時刻はntp.nict.jpと同期しています<br>
<br>
・コメントに関してはRas3072bitとAES128bitで暗号化しています
<br>
・実装予定：クレーンゲーム機能、レースゲーム<br>
<br>
・動画再生にYotube-dlを使用させて頂いています。(https://github.com/rg3/youtube-dl/)<br>
対応サイト→https://github.com/rg3/youtube-dl/blob/master/docs/supportedsites.md<br>
<br>
・開発環境:Visual Studio Community 2013<br>
<br>
使用ライブラリ<br><br>
-cient<br>
・DXライブラリ<br>
・Boost<br>
・Bullet<br>
・Crypto++<br>
・http://slis.tsukuba.ac.jp/~fujisawa.makoto.fu/様の　OBJファイル読み込み機能(http://book.mynavi.jp/support/pc/3381/)<br>
<br>
-server<br>
・Boost<br>
・Crypto++<br>
