#include "gg.h"
using namespace gg;

// �`��f�[�^
static const char filename[] = "bunny.obj";         // �O�p�`�������ꂽ Alias OBJ �`���̃t�@�C��
static GLuint nv;                                   // �ǂݍ��񂾌`��f�[�^�̒��_��
static GLfloat (*vert)[3], (*norm)[3];              // �ǂݍ��񂾌`��f�[�^�̒��_�̈ʒu�Ɩ@��
static GLuint nf;                                   // �ǂݍ��񂾌`��f�[�^�̎O�p�`��
static GLuint (*face)[3];                           // �ǂݍ��񂾌`��f�[�^�̒��_�C���f�b�N�X

// �g���b�N�{�[��
static GgTrackball *tb;

//
// �㏈��
//
static void cleanup(void)
{
  delete[] vert;
  delete[] norm;
  delete[] face;
  delete tb;
}

//
// ������
//
static void init(void)
{
  // �Q�[���O���t�B�b�N�X���_�̓s���ɂ��ƂÂ�������
  ggInit();

  // �`��f�[�^�̓ǂݍ���
  if (!ggLoadObj(filename, nv, vert, norm, nf, face, true))
  {
#if defined(_WIN32)
    MessageBox(NULL, L"�f�[�^�t�@�C�����ǂݍ��߂܂���", L"GG���_", MB_OK);
#endif
    exit(1);
  }

  // �g���b�N�{�[���̍쐬
  tb = new GgTrackball;

  // �I�������̓o�^
  atexit(cleanup);

  // �w�i�F
  glClearColor(0.2f, 0.4f, 0.6f, 0.0f);

  // �A�ʏ���
  glEnable(GL_DEPTH_TEST);

  // �A�e�t��
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);
}

//
// ��ʕ\��
//
static void display(void)
{
  // ��ʃN���A
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // ���f���r���[�ϊ��s��̕ۑ�
  glPushMatrix();

  // �g���b�N�{�[���ɂ���]
  glMultMatrixf(tb->get());

  // �}�`�̕`��
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, vert);
  glNormalPointer(GL_FLOAT, 0, norm);
  glDrawElements(GL_TRIANGLES, nf * 3, GL_UNSIGNED_INT, face);

  // ���f���r���[�ϊ��s��̕��A
  glPopMatrix();

  // �_�u���o�b�t�@�����O
  glutSwapBuffers();
}

//
// �E�B���h�E�̃��T�C�Y
//
static void resize(int w, int h)
{
  // �E�B���h�E�S�̂�\���̈�ɂ���
  glViewport(0, 0, w, h);

  // �����ϊ��s���ݒ肷��
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(60.0, (GLdouble)w / (GLdouble)h, 1.0, 5.0);

  // ���f���r���[�ϊ��s���ݒ肷��
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, 3.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

  // �g���b�N�{�[�������͈̔͂�ݒ肷��
  tb->region(w, h);
}

//
// �L�[�{�[�h�̑���
//
static void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
    // q �� Q �� ESC �ŏI������
  case 'q':
  case 'Q':
  case '\033':
    exit(0);
  default:
    break;
  }
}

//
// �������Ȃ����̏���
//
static void idle(void)
{
  // �\���̍X�V
  glutPostRedisplay();
}

// �����ꂽ�}�E�X�{�^��
static int press = 0;

//
// �}�E�X�{�^���̑���
//
static void mouse(int button, int state, int x, int y)
{
  // ���삳�ꂽ�{�^�����o���Ă���
  press = button;

  switch (button)
  {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN)
    {
      // �g���b�N�{�[���J�n
      tb->start(x, y);

      // �A�j���[�V�����J�n
      glutIdleFunc(idle);
    }
    else
    {
      // �g���b�N�{�[���C��
      tb->stop(x, y);

      // �A�j���[�V�����I��
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
// �}�E�X�̈ړ�
//
static void motion(int x, int y)
{
  switch (press)
  {
  case GLUT_LEFT_BUTTON:
    // �g���b�N�{�[���̈ړ�����
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
// ���C���v���O����
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
