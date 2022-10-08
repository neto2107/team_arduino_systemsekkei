//ボタンの生成数
int num = 0;
public class MyButton {
  private int button_id;
  private String button_label;
  private Vec2 point; //描画位置 (左上)
  private Vec2 size; //描画サイズ
  private Button bt;
  private PImage[] buttonImg = new PImage[3];//ボタン画像
  private boolean imgflag=false;

  //コンストラクタ
  MyButton(String label, Vec2 point, Vec2 size) {
    this.button_label =label;
    this.point = new Vec2(point);
    this.size = new Vec2(size);

    //ボタンの生成数を1増やす
    num ++;
    button_id =num;
  }
  //画像セット(非選択時の画像、ホバー時の画像、押し下げ時の画像、)
  public void setImg(String path1, String path2, String path3) {

    buttonImg[0] =  loadImage(path1);
    buttonImg[1] =  loadImage(path2);
    buttonImg[2] =  loadImage(path3);
    imgflag=true;
  }
  //画像をセット（すべて同じ画像）
  public void setImg(String imgPath) {
    for (int i = 0; i<3; i++) {
      buttonImg[i] =  loadImage(imgPath);
    }
    imgflag=true;
  }
  //ボタンを追加
  public void addButton() {
    if (imgflag == true) {
      // ボタンの生成
      bt = cp.addButton(button_label).setPosition(point.x, point.y).setSize((int)size.x, (int)size.y).setImages(buttonImg).updateSize();
    } else {
      bt = cp.addButton(button_label).setPosition(point.x, point.y).setSize((int)size.x, (int)size.y);
    }
  }


  public void setVisible(boolean flag) {
    bt.setVisible(flag);
  }
}
