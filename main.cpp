#include <QApplication>
#include <QWebView>

int main(int argc, char ** argv)
{
  QApplication app(argc, argv);
  QWebView view;
  view.setAttribute(Qt::WA_TranslucentBackground, true);
  view.show();
  view.load(QUrl(argv[1]));

  return app.exec();
}
