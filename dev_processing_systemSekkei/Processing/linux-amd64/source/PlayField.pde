public Robot robot1;
public Robot robot2;
public Robot robot3;

public class PlayField extends Field {

  private Vec4 left_goal_line;
  private Vec4 right_goal_line;
  private Arrow ar;

  PlayField() {


    left_goal_line = new Vec4(field_pos.x, 143, field_pos.x, 143 + 475);
    right_goal_line = new Vec4(field_pos.x + local_field_size.x, 143, field_pos.x + local_field_size.x, 143+475);
    //ロボットのインスタンス生成

    ar = new Arrow(new Vec2(300, 300), new Vec2(400, 400), 0, 0, 255);
  }

  public void draw() {
    strokeWeight(10);
    //フィールドの描画
    fill(255, 255, 255);
    rect(field_pos.x, field_pos.y, local_field_size.x, local_field_size.y, 60);
    //ゴールの描画

    stroke(255, 0, 0);
    line(left_goal_line.x1, left_goal_line.y1, left_goal_line.x2, left_goal_line.y2);
    stroke(0, 0, 255);
    line(right_goal_line.x1, right_goal_line.y1, right_goal_line.x2, right_goal_line.y2);
    stroke(0, 0, 0);
    strokeWeight(1);

    //ロボットの描画
    robot1.draw();
    robot2.draw();
    robot3.draw();
    ar.draw();
  }
}
