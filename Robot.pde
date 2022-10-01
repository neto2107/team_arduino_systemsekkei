
public class Robot extends Field {
  private int id;
  private Vec2 real_pos;
  private Vec2 local_pos;
  private Vec2 draw_pos;
  
  Robot(Vec2 real_pos){
    this.real_pos = real_pos;
    this.local_pos = this.convert_real_for_local(real_pos);
    println(this.local_pos.x + "," + this.local_pos.y);
    this.draw_pos = this.convertDrawPos(local_pos);
  }
  
  public void draw(){
     ellipse(draw_pos.x,draw_pos.y,50,50);
  }
  
  
}
