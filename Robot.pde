public int robot_num=1;
public class Robot extends Field {
  private int id;
  private Vec2 real_pos;
  private float angle; //角度 //ラジアン角
  private float accel;;//加速度
  private float sensing_range =300; //リアルでどれだけ遠くのものに反応するか(mm)
  color c3 = color(255,255,255); //カラーセンサーの取得値
  
  private Vec2 local_pos;
  private Vec2 draw_pos;
  private float local_sensing_range;//画面上で、どれだけ遠くのものに反応するか
  
  color c1 = color(1,61,255,70); //超音波センサーセンシング中の色
  color c2 = color(255,0,0,70);


  Textlabel tl1;
   //コンストラクタ
  Robot(Vec2 real_pos,float degree) {
    this.real_pos = real_pos;
    this.local_pos = this.convert_real_for_local(real_pos);
    println(this.local_pos.x + "," + this.local_pos.y);
    this.draw_pos = this.convertDrawPos(local_pos);
    this.angle = Utils.deg_to_rad(degree);
    this.local_sensing_range = Utils.map(300,0,2500,0,1000); 

    id=robot_num;
    robot_num++; //idを更新
  }
  
  //ロボットの向きを表示する
  

  //ロボットを描画するメソッド
  public void draw() {
    pushMatrix(); //(0,0)を原点とする座標軸をスタックに格納
    translate(draw_pos.x,draw_pos.y); //座標軸をロボットの位置まで移動
    //外円を描画
    fill(c3);
    ellipse(0,0,50,50);
    //内円を描画
    fill(255, 255, 255);
    ellipse(0, 0, 40, 40);
    //機体番号を描画
    fill(0, 0, 0);
    textSize(20);
    text (Integer.toString(id), -5, -13, 50, 50);
    //超音波センサーの位置を描画
    rotate(angle);//座標軸を回転させる
    fill(c1);
    arc(0,0,local_sensing_range,local_sensing_range,Utils.deg_to_rad(-105),Utils.deg_to_rad(-75));
    popMatrix();
  }

  public void setPos(Vec2 real_pos) {
    this.real_pos=real_pos;
    updatePos();
  }
  
  //ロボットの角度を更新するセッター(入力がラジアン角)
  public void setAngle(float angle){
    this.angle = angle;
  }
    //ロボットの角度を更新するセッター(入力がが度)
  public void set_degree(float degree){
        this.angle = Utils.deg_to_rad(degree);
  }

  public void updatePos() {
    this.local_pos = this.convert_real_for_local(real_pos);
    this.draw_pos = this.convertDrawPos(local_pos);
  }
  
  //カラーセンサーの取得地をセット
  public void setColorSenserValue(color c){
     this.c3 = c;
  }
}
