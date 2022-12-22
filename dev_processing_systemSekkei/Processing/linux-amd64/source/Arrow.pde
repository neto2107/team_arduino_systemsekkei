public class Arrow {
  private int arrowLength = 10; //矢印の長さ
  private float arrowAngle= 0.5;//矢印の開き具合
  private float angle;
  private Vec2 start_pos;
  private Vec2 end_pos;

  private int[] arrow_color= new int[3];

  Arrow(Vec2 start_pos, Vec2 end_pos, int color_r, int color_g, int color_b) {
    this.start_pos = start_pos;
    this.end_pos = end_pos;
    angle = atan2(end_pos.y - start_pos.y, end_pos.x-start_pos.x);
    arrow_color[0]=color_r;
    arrow_color[1]=color_g;
    arrow_color[2]=color_b;
  }

  public void set_start_pos(Vec2 start_pos) {
    this.start_pos = start_pos;
    update_field();
  }

  public void set_end_pos(Vec2 end_pos) {
    this.end_pos = end_pos;
    update_field();
  }

  private  void update_field() {
    angle = atan2(end_pos.y - start_pos.y, end_pos.x-start_pos.x);
  }

  public void draw() {

    stroke(arrow_color[0], arrow_color[1], arrow_color[2]);
    line(start_pos.x, start_pos.y, end_pos.x, end_pos.y);
    pushMatrix();
    translate(end_pos.x, end_pos.y);
    rotate(angle);
    line(0, 0, -arrowLength*cos(arrowAngle), arrowLength*sin(arrowAngle));
    line(0, 0, -arrowLength*cos(arrowAngle), -arrowLength*sin(arrowAngle));
    popMatrix();
    stroke(0, 0, 0);
  }
}
