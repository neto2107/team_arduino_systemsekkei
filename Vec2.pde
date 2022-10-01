public class Vec2 {
  public float x, y;
  //コンストラクタ
  public Vec2(float x, float y) {
    this.x = x;
    this.y = y;
  }

  //ゼロベクトルで定義
  public Vec2() {
    this(0, 0);
  }
  //ベクトルをコピー
  public Vec2(Vec2 v) {
    this(v.x, v.y);
  }
  /**
   * 値をセットします。
   *
   * @param x
   * x値
   * @param y
   * y値
   */
  public void set(float x, float y) {
    this.x = x;
    this.y = y;
  }

  /**
   * 値を0にリセットします。
   */
  public void zero() {
    set(0, 0);
  }

  /**
   * ベクトルを加算します。
   *
   * @param v
   * 加算するベクトル
   * @return 加算されたベクトル
   */
  public Vec2 add(Vec2 v) {
    x += v.x;
    y += v.y;
    return this;
  }

  /**
   * ベクトルを減算します。
   *
   * @param v
   * 減算するベクトル
   * @return 減算されたベクトル
   */
  public Vec2 sub(Vec2 v) {
    x -= v.x;
    y -= v.y;
    return this;
  }

  /**
   * ベクトルを乗算します。
   *
   * @param v
   * 乗算するベクトル
   * @return 乗算されたベクトル
   */
  public Vec2 mul(float f) {
    x *= f;
    y *= f;
    return this;
  }

  /**
   * ベクトルを除算します。
   *
   * @param v
   * 除算するベクトル
   * @return 除算されたベクトル
   */
  public Vec2 div(float f) {
    if (f == 0)
      return this;
    x /= f;
    y /= f;
    return this;
  }

  /**
   * ベクトルを加算した結果を返します。
   *
   * @param v
   * 加算するベクトル
   * @return 加算されたベクトル
   */
  public Vec2 getAdd(Vec2 v) {
    return new Vec2(x + v.x, y + v.y);
  }

  /**
   * ベクトルを減算した結果を返します。
   *
   * @param v
   * 減算するベクトル
   * @return 減算されたベクトル
   */
  public Vec2 getSub(Vec2 v) {
    return new Vec2(x - v.x, y - v.y);
  }

  /**
   * ベクトルを乗算した結果を返します。
   *
   * @param v
   * 乗算するベクトル
   * @return 乗算されたベクトル
   */
  public Vec2 getMul(float f) {
    return new Vec2(x * f, y * f);
  }

  /**
   * ベクトルを除算した結果を返します。
   *
   * @param v
   * 除算するベクトル
   * @return 除算されたベクトル
   */
  public Vec2 getDiv(float f) {
    if (f == 0)
      return this;
    return new Vec2(x / f, y / f);
  }

  /**
   * ベクトルの長さを返します。
   *
   * @return このベクトルの長さ
   */
  public float getLength() {
    return (float) Math.sqrt(x * x + y * y);
  }

  /**
   * ベクトルを正規化します。
   
   * 正規化されたベクトルは getLength() == 1 という条件を見たします。
   
   * ただし非常に長さが短いときや、長さが0のときは正しく正規化されません。
   */
  public void normalize() {
    float length = getLength();
    if (length < 0.0001f)
      length = 1.0f;
    x /= length;
    y /= length;
  }

  /**
   * 正規化されたベクトルを返します。
   *
   * @return 正規化されたベクトル
   * @see #normalize()
   */
  public Vec2 getNormalize() {
    float length = getLength();
    if (length < 0.0001f)
      length = 1.0f;
    return new Vec2(x / length, y / length);
  }

  /**
   * ベクトルを反転させます。
   */
  public void reverse() {
    x *= -1;
    y *= -1;
  }

  /**
   * 反転したベクトルが返されます。
   *
   * @return 反転されたベクトル
   */
  public Vec2 getReverse() {
    return new Vec2(-x, -y);
  }

  /**
   * ベクトルをラジアン角で回転させます。
   
   * 度数からラジアンへの変更は Mathf.toRadians が使えます。
   *
   * @param radian
   * ラジアン単位の角度
   * @see Mathf#toRadians(float degree)
   */
  public void rotate(float radian) {
    float x2 = (float) (-Math.sin(radian) * y + Math.cos(radian) * x);
    float y2 = (float) (Math.cos(radian) * y + Math.sin(radian) * x);
    x = x2;
    y = y2;
  }

  /**
   * ベクトルをラジアン角で回転させた結果を返します。
   
   * 度数からラジアンへの変更は Mathf.toRadians が使えます。
   *
   * @param radian
   * ラジアン単位の角度
   * @return 回転後のベクトル
   * @see Mathf#toRadians(float degree)
   */
  public Vec2 getRotate(float radian) {
    return new Vec2((float) (-Math.sin(radian) * y + Math.cos(radian) * x),
      (float) (Math.cos(radian) * y + Math.sin(radian) * x));
  }

  /**
   * @see Object#clone()
   */
  public Vec2 clone() {
    return new Vec2(x, y);
  }

  /**
   * 二つのベクトルの外積を返します。
   
   * 外積は二つのベクトルによって作られる平行四辺形の面積と等しくなります。
   *
   * @param v
   * 一つ目のベクトル
   * @param v2
   * 二つ目のベクトル
   * @return 外積
   */
  public float cross(Vec2 v, Vec2 v2) {
    return v.x * v2.y - v.y * v2.x;
  }

  /**
   * ベクトルの一つが3Dだと仮定したときに、そのz値を指定して外積のベクトルを返します。
   *
   * @param z
   * z値
   * @param v
   * ベクトル
   * @return 外積
   */
  public Vec2 cross(float z, Vec2 v) {
    return new Vec2(-z * v.y, z * v.x);
  }

  /**
   * ベクトルの一つが3Dだと仮定したときに、そのz値を指定して外積のベクトルを返します。
   *
   * @param v
   * ベクトル
   * @param z
   * z値
   * @return 外積
   */
  public Vec2 cross(Vec2 v, float z) {
    return new Vec2(-z * v.y, z * v.x);
  }

  /**
   * 二つのベクトルの内積を返します。
   *
   * @param v
   * 一つ目のベクトル
   * @param v2
   * 二つ目のベクトル
   * @return 内積
   */
  public float dot(Vec2 v, Vec2 v2) {
    return v.x * v2.x + v.y * v2.y;
  }

  /**
   * 長さの二乗を返します。
   *
   * @return 長さの二乗
   */
  public float getLengthSquare() {
    return x * x + y * y;
  }

  /**
   * @see Object#toString()
   */
  public String toString() {
    return "X=" + x + ", Y=" + y;
  }
}

public class Vec4 {
  float x1, y1, x2, y2;
  //コンストラクタ
  public Vec4(float x1, float y1, float x2, float y2) {
    this.x1 = x1;
    this.y1 = y1;
    this.x2 = x2;
    this.y2 = y2;
  }

  //ゼロベクトルで定義
  public Vec4() {
    this(0, 0, 0, 0);
  }
  //ベクトルをコピー
  public Vec4(Vec4 v) {
    this(v.x1, v.y1, v.x2, v.y2);
  }
  /**
   * 値をセットします。
   *
   * @param x
   * x値
   * @param y
   * y値
   */
  public void set(float x1, float y1, float x2, float y2) {
    this.x1 = x1;
    this.y1 = y1;
    this.x2 = x2;
    this.y2 = y2;
  }

  /**
   * 値を0にリセットします。
   */
  public void zero() {
    set(0, 0, 0, 0);
  }
}
