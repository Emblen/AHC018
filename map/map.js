// draw_canvas 関数の定義
function draw_canvas() {
    // 2次元描画用のコンテキストを作成し c1 に代入する。
    var canvas1 = document.getElementById('canvas_tag_1');
    if ( ! canvas1 || ! canvas1.getContext ) { return false; }
    var c1 = canvas1.getContext('2d');
    // var data = fileinput();
    // 描画の開始
    c1.beginPath();
    var i,j;
    // x 方向にi=0～14まで15マスを描画する
    for (i = 0; i < 200; i++) {
        // y 方向にy=0～14まで15マスを描画する
    for (j = 0; j < 200; j++) {
        c1.beginPath();
          // i の値によって r(赤)の輝度を変化させる
          // toString(10)で、文字列に変換
          var red = (i*0.4+j*1).toString(10); 
        //   if(data[200*i+j]==0) red = (30).toString(10);
        //   else if(data[200*i+j]==1) red = (150).toString(10);
        //   else red = (255).toString(10);
          c1.fillStyle = 'rgb(' + red + ',0,0)';
          // i,j を座標に変換
          c1.rect(i * 3, j * 3, 2.8, 2.8);
          c1.fill(); // 色を塗る
      }
    }
}

function fileinput(){
    var fs = new ActiveXObject("Scripting.FileSystemObject");
    var inputfile = fs.OpenTextFile("mapout.txt", 2);
    var text = "";
    for(var i=0; i<200; i++){
        var str = inputfile.ReadLine();
        text += str;
    }
    return text;
} 