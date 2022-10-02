public static class Utils {
  //数値をある範囲から、別の範囲に変換する
  public static float map (float x, float in_min, float in_max, float out_min, float out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }
  
  //ラジアン角から度への変換
  public static float rad_to_deg(float radian){
    return radian * 180 /PI;
  }
  //度からラジアン角へ変換
  public static float deg_to_rad(float degree){
     return degree * PI /180; 
  }
}
