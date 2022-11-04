public class SideBar {
  final private int[][] box_pos ={{3, 10, 47, 50}, {3, 71, 47, 50}, {3, 135, 47, 50}, {3, 202, 47, 50}};
  private int barWidth;
  private int select_num = 0;

  SideBar(int barWidth) {
    this.barWidth = barWidth;
  }
  public void barDraw() {
    //サイドバーの描画
    fill(48, 48, 48);
    rect(0, 0, barWidth, height);
    selectedBoxDraw();
  }


  private void selectedBoxDraw() {
    fill(161, 161, 161);

    rect(box_pos[select_num][0], box_pos[select_num][1], box_pos[select_num][2], box_pos[select_num][3], 5, 0, 0, 5);
  }

  public int getselect_num() {
    return select_num;
  }

  //サイドバーのどれが選択されているかをセットするセッター
  //select_numは0~3のintがた
  public void setSelectNum(int select_num) {
    if (select_num>=0&&select_num<=3) {
      this.select_num =select_num;
    } else {
      println("select_numの値が不正です");
    }
  }
}
