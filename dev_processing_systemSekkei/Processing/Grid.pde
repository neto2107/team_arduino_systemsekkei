//グリッドの左上の位置
public float[] ancerPointListx = new float[6];
public float[] ancerPointListy = new float[6];
//グリッドのサイズ
public float[][] gridSize = new float [6][2];

public class Grid extends View {
  private final int[] grid_row={4, 2} ;

  public Grid() {
    Vec2 ancerPoint = convertDrawPos(new Vec2(0, 0));
    for (int i=0; i<grid_row[0]; i++) {
      print(i);
      ancerPointListx[i] =ancerPoint.x + ( view_size.x   /grid_row[0] ) * i;
      ancerPointListy[i]  =0;
      gridSize[i][0] =  view_size.x   /grid_row[0];
      gridSize[i][1] = view_size.y  / 2;
    }
    for (int j = grid_row[0]; j<grid_row[1]+grid_row[0]; j++) {
      ancerPointListx[j] =ancerPoint.x + ( view_size.x   /grid_row[1] ) * (j-grid_row[0]);
      ancerPointListy[j]  =view_size.y / 2;
            gridSize[j][0] =  view_size.x   /grid_row[1];
      gridSize[j][1] = view_size.y  / 2;
    }
  }
  protected Vec2 convertDrawPos(Vec2 local_pos) {
    return view_point.getAdd(local_pos);
  }



  public void draw() {

    for (int i=0; i<4; i++) {
      stroke(0, 0, 0);
      fill(242, 242, 242);
      rect(ancerPointListx[i], ancerPointListy[i], gridSize[i][0], gridSize[i][1]);
    }
    for (int i = 4; i<6; i++) {
      rect(ancerPointListx[i], ancerPointListy[i], gridSize[i][0], gridSize[i][1]);
    }
  }
}
