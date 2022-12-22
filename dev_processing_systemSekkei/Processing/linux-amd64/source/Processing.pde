import controlP5.*;
import processing.serial.*;
//定数の宣言
final int height = 720;
final int width = 1280;

final int sideBar_width = 50;

//変数の宣言
Serial port_G;//シリアル「ポート
Serial port2_G;
Serial port3_G;

MyButton button_field;
MyButton button_robot1;
MyButton button_robot2;
MyButton button_robot3;
SideBar sideBar;
PlayField play_field;
Grid grid;

GridContents contents1;
GridContents contents2;
GridContents contents3;

ControlP5 cp;
void setup() {
  size(1280, 720);
  //インスタンス生成
  sideBar = new SideBar(sideBar_width);
  //controlP5を使用する
  cp = new ControlP5(this);
  play_field = new PlayField();

  //ロボットの生成
  robot1 = new Robot(new Vec2(100, 800), 0);
  robot2 = new Robot(new Vec2(300, 100), 90);
  robot3 = new Robot(new Vec2(300, 1500),-90);
  
  //Serialポートを指定 
  port_G = new Serial(this,"/dev/tty.usbserial-AL01T41S",9600);


  //グリッドの生成
  grid = new Grid();

  //グリッドに描画する要素の作成
  contents1=new GridContents(robot1);
  contents2=new GridContents(robot2);
  contents3=new GridContents(robot3);



  //ボタン関係の描画
  DrawSideButton();
}

void draw() {
  //サイドバーの描画
  background(242, 242, 242);
  sideBar.barDraw();

  switch(sideBar.getselect_num()  ) {
  case 0:
    play_field .draw();
    break;
  case 1:
    grid.draw();
    contents1.draw();


    break;
  case 2:
    grid.draw();
    contents2.draw();

    break;
  case 3:
    grid.draw();
    contents3.draw();
    break;
  }
}

//イベントが発生した場合の処理
public void controlEvent(ControlEvent theEvent) {
  String label = theEvent.getController().getName();
  switch(label) {
  case "field":
    sideBar.setSelectNum(0);
    break;
  case "robot1":
    sideBar.setSelectNum(1);
    break;
  case "robot2":
    sideBar.setSelectNum(2);
    break;
  case "robot3":
    sideBar.setSelectNum(3);
    break;
  }
}


//ボタンの描画
void DrawSideButton() {
  //インスタンス生成
  button_field = new MyButton("field", new Vec2(6, 19), new Vec2(40, 32));
  button_robot1 = new MyButton("robot1", new Vec2(2, 71), new Vec2(47, 47));
  button_robot2 = new MyButton("robot2", new Vec2(2, 137), new Vec2(47, 47));
  button_robot3 = new MyButton("robot3", new Vec2(2, 202), new Vec2(47, 47));

  //イメージの読み込み
  button_field.setImg("field.png");
  button_robot1.setImg("robot_button1.png");
  button_robot2.setImg("robot_button2.png");
  button_robot3.setImg("robot_button3.png");
  //ボタンの描画
  button_field.addButton();
  button_robot1.addButton();
  button_robot2.addButton();
  button_robot3.addButton();
}
