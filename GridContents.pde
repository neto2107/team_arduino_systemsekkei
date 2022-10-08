
public class GridContents extends Grid {
  protected Vec2[] ancerPoint;    //それぞれのグリッドの左上の地点
  protected Vec2[] size; //それぞれのグリッドのサイズ
  protected Vec2[] centerPoint; //それぞれのグリッドの中心
  protected Robot robot;

  color c1 = color(1, 61, 255, 70); //超音波センサーセンシング中の色

  GridContents(Robot robot) {
    ancerPoint = new Vec2[6];
    size = new Vec2[6];
    centerPoint = new Vec2[6];
    //それぞれのグリッドの左上
    for (int i = 0; i < 6; i++) {
      ancerPoint[i] = new Vec2(ancerPointListx[i], ancerPointListy[i]);
      size[i] = new Vec2(gridSize[i][0], gridSize[i][1]);
      centerPoint[i] = ancerPoint[i].getAdd(new Vec2(size[i].x /2, size[i].y/2));
    }
    //描画に必要なロボットの情報
    this.robot=robot;
  }

  public void draw() {
    this.DrawUltrasonicSensor();
    this.drawColorSensor();
  }

  //pushMatrix();
  //translate(centerPoint.x,centerPoint.y);
  ////ここに描画処理を記述
  //ellipse(0,0,30,30);
  //popMatrix();


  //超音波センサーの描画
  private void DrawUltrasonicSensor() {
    pushMatrix();
    translate(centerPoint[0].x, centerPoint[0].y);
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
    } else {
      fill(0, 0, 0);
      textSize(30);
      text ( "Not  detected", -90, 130, 200, 100);
    }

    popMatrix();
  }

  //カラーセンサーの描画
  private void drawColorSensor() {
    pushMatrix();
    pushStyle();
    translate(centerPoint[1].x, centerPoint[1].y);
    //タイトルを描画
    fill(0, 0, 0);
    textSize(30);
    text ("Color Sensor", -80, -150, 500, 200);


    //色の表示部分を描画
    color sensored_color = robot.getColorSensorValue();
    fill(sensored_color);
    ellipse(0, 25, 200, 200);

    //色判定部分の描画
    int judge = robot.getColorSenserJudged();
    fill(0, 0, 0);
    textSize(50);
    switch(judge) {
    case 0://判定値が黒なら
      text ("Black", -55, -5, 150, 100);
      break;
    case 1: //判定値が白なら
      text ("White", -55, -5, 150, 100);
      break;
    case 2:
      text ("Red", -55, -5, 150, 100);
      break;
    case 3:
      text("Blue", -55, -5, 150, 100);
      break;
    }
    popStyle();
    popMatrix();
  }

  //地磁気センサーの描画
  private void drawGeomagneticSensor() {
    pushMatrix();
    pushStyle();
    translate(centerPoint[2].x, centerPoint[2].y);
    popStyle();
    popMatrix();
  }
}
