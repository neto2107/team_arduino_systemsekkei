public int robot_num=1;
public class Robot extends Field {
  private int id; //ロボットの分類id
  private Vec2 real_pos; //ロボットの実際の位置
  private int accel;//ロボットの加速度
  private float angle; //角度 //ラジアン角
  private int ultrasonic_sensing_distance = 0;//センサーの感知した位置(mm) 0は未反応
  private int ultrasonic_sensing_range =300; //リアルでどれだけ遠くのものに反応するか(mm)
  private color c3 = color(255, 255, 255); //カラーセンサーの取得値
  private int colorSenserJudged = 0; //カラーセンサーの読み取り値 (0:白 1:黒 2:赤,3：青)

  private Vec2 local_pos;
  private Vec2 draw_pos;
  private float local_sensing_range;//画面上で、どれだけ遠くのものに反応するか

  color c1 = color(1, 61, 255, 70); //超音波センサーセンシング中の色
  color c2 = color(255, 0, 0, 70);


  Textlabel tl1;
  //コンストラクタ
  Robot(Vec2 real_pos, float degree) {
    this.real_pos = real_pos;
    this.local_pos = this.convert_real_for_local(real_pos);
    println(this.local_pos.x + "," + this.local_pos.y);
    this.draw_pos = this.convertDrawPos(local_pos);
    this.angle = Utils.deg_to_rad(degree);
    this.local_sensing_range = Utils.map(300, 0, 2500, 0, 1000);
    this.accel = 0;
    id=robot_num;
    robot_num++; //idを更新
  }

  //ロボットの向きを表示する


  //ロボットを描画するメソッド
  public void draw() {
    pushMatrix(); //(0,0)を原点とする座標軸をスタックに格納
    translate(draw_pos.x, draw_pos.y); //座標軸をロボットの位置まで移動
    //外円を描画
    fill(c3);
    ellipse(0, 0, 50, 50);
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
    arc(0, 0, local_sensing_range, local_sensing_range, Utils.deg_to_rad(-105), Utils.deg_to_rad(-75));
    popMatrix();
  }
  
  public int getId(){
    return id;
  }

  public void setPos(Vec2 real_pos) {
    this.real_pos=real_pos;
    updatePos();
  }

  //ロボットの角度を更新するセッター(入力がラジアン角)
  public void setAngle(float angle) {
    this.angle = angle;
  }
  
  
  //ロボットの角度を更新するセッター(入力がが度)
  public void set_degree(float degree) {
    this.angle = Utils.deg_to_rad(degree);
  }
  public float getAngle(){
    return this.angle;
  }
  

  public void updatePos() {
    this.local_pos = this.convert_real_for_local(real_pos);
    this.draw_pos = this.convertDrawPos(local_pos);
  }

  //カラーセンサーの取得地をセット
  public void setColorSensorValue(color c) {
    this.c3 = c;
  }
  public color getColorSensorValue(){
    return this.c3;
  }
  
  //カラーセンサーの判定値のゲッターとセッター
  public void setColorSenserJudged(int value){
    if(value >= 0 || value <+ 3){ //値が規定内ならセット
      colorSenserJudged = value;
    }else{
      print("error:ColorSenserJudgedが規定外の値です");
    }
  }
  
  public int  getColorSenserJudged(){
    return this.colorSenserJudged;
  }
  
  
  //超音波センサーの値のセッターとゲッター
  public int getUltrasonicSensingDistance(){
    return ultrasonic_sensing_distance;
  }
  public void setUltrasonicSensingDistance(int distance){
    this.ultrasonic_sensing_distance = distance;
  }
  
  //加速度センサーの値のセッターとゲッター
  public void setAccel(int accel){
    this.accel = accel;
  }
  public int getAccel(){
    return this.accel;
  }
}
