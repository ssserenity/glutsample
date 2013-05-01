#include "gg.h"
using namespace gg;

// 形状データ
static const char filename[] = "bunny.obj";         // 三角形分割された Alias OBJ 形式のファイル
static GLuint nv;                                   // 読み込んだ形状データの頂点数
static GLfloat (*vert)[3], (*norm)[3];              // 読み込んだ形状データの頂点の位置と法線
static GLuint nf;                                   // 読み込んだ形状データの三角形数
static GLuint (*face)[3];                           // 読み込んだ形状データの頂点インデックス

// トラックボール
static GgTrackball *tb;

//
// 後処理
//
static void cleanup(void)
{
  delete[] vert;
  delete[] norm;
  delete[] face;
  delete tb;
}

//
// 初期化
//
static void init(void)
{
  // ゲームグラフィックス特論の都合にもとづく初期化
  ggInit();

  // 形状データの読み込み
  if (!ggLoadObj(filename, nv, vert, norm, nf, face, true))
  {
#if defined(_WIN32)
    MessageBox(NULL, L"データファイルが読み込めません", L"GG特論", MB_OK);
#endif
    exit(1);
  }

  // トラックボールの作成
  tb = new GgTrackball;

  // 終了処理の登録
  atexit(cleanup);

  // 背景色
  glClearColor(0.2f, 0.4f, 0.6f, 0.0f);

  // 陰面消去
  glEnable(GL_DEPTH_TEST);

  // 陰影付け
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}

//
// 画面表示
//
static void display(void)
{
  // 画面クリア
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // モデルビュー変換行列の保存
  glPushMatrix();

  // トラックボールによる回転
  glMultMatrixf(tb->get());

  // 図形の描画
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vert);
  glNormalPointer(GL_FLOAT, 0, norm);
  glDrawElements(GL_TRIANGLES, nf * 3, GL_UNSIGNED_INT, face);

  // モデルビュー変換行列の復帰
  glPopMatrix();

  // ダブルバッファリング
  glutSwapBuffers();
}

//
// ウィンドウのリサイズ
//
static void resize(int w, int h)
{
  // ウィンドウ全体を表示領域にする
  glViewport(0, 0, w, h);

  // 透視変換行列を設定する
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLdouble)w / (GLdouble)h, 1.0, 5.0);

  // モデルビュー変換行列を設定する
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  // トラックボール処理の範囲を設定する
  tb->region(w, h);
}

//
// キーボードの操作
//
static void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
    // q か Q か ESC で終了する
  case 'q':
  case 'Q':
  case '\033':
    exit(0);
  default:
    break;
  }
}

//
// 何もしない時の処理
//
static void idle(void)
{
  // 表示の更新
  glutPostRedisplay();
}

// 押されたマウスボタン
static int press = 0;

//
// マウスボタンの操作
//
static void mouse(int button, int state, int x, int y)
{
  // 操作されたボタンを覚えておく
  press = button;

  switch (button)
  {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN)
    {
      // トラックボール開始
      tb->start(x, y);

      // アニメーション開始
      glutIdleFunc(idle);
    }
    else
    {
      // トラックボール修了
      tb->stop(x, y);

      // アニメーション終了
      glutIdleFunc(0);
    }
    break;
  case GLUT_MIDDLE_BUTTON:
    break;
  case GLUT_RIGHT_BUTTON:
    break;
  default:
    break;
  }
}

//
// マウスの移動
//
static void motion(int x, int y)
{
  switch (press)
  {
  case GLUT_LEFT_BUTTON:
    // トラックボールの移動処理
    tb->motion(x, y);
    break;
  case GLUT_MIDDLE_BUTTON:
    break;
  case GLUT_RIGHT_BUTTON:
    break;
  default:
    break;
  }
}

//
// メインプログラム
//
int main(int argc, char *argv[])
{
  glutInitWindowSize(800, 800);
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_DOUBLE);
  glutCreateWindow("Gg Library Sample for GLUT");
  glutReshapeFunc(resize);
  glutKeyboardFunc(keyboard);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutDisplayFunc(display);
  init();
  glutMainLoop();
  return 0;
}
