
public  abstract class GridContents extends Grid {
  protected Vec2 ancerPoint;    //それぞれのグリッドの左上の地点
  protected Vec2 size; //それぞれのグリッドのサイズ
  protected Vec2 centerPoint; //それぞれのグリッドの中心
  protected Robot robot;

  color c1 = color(1, 61, 255, 70); //超音波センサーセンシング中の色
   int gridContentsNum = 0; //コンテンツがいくつ作られているか
  GridContents(Robot robot,int contentsNum) {
    this.gridContentsNum = contentsNum;
    //それぞれのグリッドの左上
    ancerPoint = new Vec2(ancerPointListx[gridContentsNum], ancerPointListy[gridContentsNum]);
    size = new Vec2(gridSize[gridContentsNum][0], gridSize[gridContentsNum][1]);
    centerPoint = ancerPoint.getAdd(new Vec2(size.x /2, size.y/2));

    //描画に必要なロボットの情報
    this.robot=robot;
  }

  public abstract void draw();
  //pushMatrix();
  //translate(centerPoint.x,centerPoint.y);
  ////ここに描画処理を記述
  //ellipse(0,0,30,30);
  //popMatrix();
}

public class DrawUltrasonicSensor extends GridContents {
  DrawUltrasonicSensor(Robot robot) {
    super(robot,0); //スーパーコンストラクタ
  }

  @Override
    public void draw() {
    pushMatrix();
    translate(centerPoint.x, centerPoint.y);
    //ここに描画処理を記述

    //タイトルを描画
    fill(0, 0, 0);
    textSize(30);
    text ("ultrasonic sensor", -110, -150, 500, 200);

    //ロボットを描画
    pushMatrix();
    translate(0, 100);
    //外円を描画
    fill(255, 255, 255);
    ellipse(0, 0, 40, 40);
    //内円を描画
    fill(255, 255, 255);
    ellipse(0, 0, 30, 30);
    //機体番号を描画
    fill(0, 0, 0);
    textSize(20);
    text (Integer.toString(robot.id), -5, -13, 50, 50);
    popMatrix();

    //センサー範囲を描画
    line(0, 80, 0, -100);
    fill(c1);
    strokeWeight(0);
    triangle(0, 80, -30, -100, 30, -100);
    strokeWeight(1);


    //センサー位置を描画
    int censorDir = robot.getUltrasonicSensingDistance();
    if (censorDir >0) {
      fill(255, 0, 0);
      float drawpos = map(censorDir, 0, 300, 80, -100);
      ellipse(0, drawpos, 10, 10);
      //距離を描画
      fill(0, 0, 0);
      textSize(30);
      text (Integer.toString(censorDir) + "mm", -50, 130, 200, 100);
    }else{
      fill(0, 0, 0);
      textSize(30);
      text ( "Not  detected", -90, 130, 200, 100);
    }

  popMatrix();
  }
}
