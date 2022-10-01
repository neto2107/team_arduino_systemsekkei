public int robot_num=1;
public class Robot extends Field {
  private int id;
  private Vec2 real_pos;
  private float angle; //角度
  private float accel;;//加速度
  
  
  private Vec2 local_pos;
  private Vec2 draw_pos;

  Textlabel tl1;

  Robot(Vec2 real_pos) {
    this.real_pos = real_pos;
    this.local_pos = this.convert_real_for_local(real_pos);
    println(this.local_pos.x + "," + this.local_pos.y);
    this.draw_pos = this.convertDrawPos(local_pos);

    id=robot_num;
    robot_num++; //idを更新
  }

  //ロボットを描画するメソッド
  public void draw() {
    fill(255, 255, 255);
    ellipse(draw_pos.x, draw_pos.y, 50, 50);
    fill(0, 0, 0);
    textSize(20);
    text (Integer.toString(id), draw_pos.x-5, draw_pos.y-13, 50, 50);
  }

  public void setPos(Vec2 real_pos) {
    this.real_pos=real_pos;
    updatePos();
  }

  public void updatePos() {
    this.local_pos = this.convert_real_for_local(real_pos);
    this.draw_pos = this.convertDrawPos(local_pos);
  }
}
