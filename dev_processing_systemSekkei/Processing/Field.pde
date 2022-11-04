public abstract class Field extends View {
  final protected Vec2 real_field_size = new Vec2(2500, 1600); //実際のフィールドサイズ(単位はmm) w*h
  protected Vec2 local_field_size = new Vec2(1000, 640);
  ;
  protected Vec2 field_pos= new Vec2(195, 40);

  public Vec2 convert_real_for_local(Vec2 real_pos) {
    return new Vec2(map(real_pos.x, 0, real_field_size.x, 0, local_field_size.x), map(real_pos.y, 0, real_field_size.y, 0, local_field_size.y));
  }
  protected Vec2 convertDrawPos(Vec2 local_pos) {
    return field_pos.getAdd(local_pos);
  }

  protected abstract void draw();
}

public abstract class View {
  protected Vec2 view_size = new Vec2(width-sideBar_width, height);
  protected Vec2 view_point = new Vec2(sideBar_width, 0);
}
