#ifndef _QUADRO_UTIL_CPP
#define _QUADRO_UTIL_CPP

#include "Util.hpp"

std::ostream &
operator<<(std::ostream & os, Quadro::Direction d)
{
  switch (d) {
    case Quadro::Left:
      return os << "Left";
    case Quadro::Right:
      return os << "Right";
    case Quadro::Top:
      return os << "Top";
    case Quadro::Bottom:
      return os << "Bottom";
    case Quadro::TopLeft:
      return os << "TopLeft";
    case Quadro::BottomRight:
      return os << "BottomRight";
    case Quadro::TopRight:
      return os << "TopRight";
    case Quadro::BottomLeft:
      return os << "BottomLeft";
    case Quadro::None:
      return os << "None";
  };

  return os;
}

void
Quadro::withAllChildren(QObject * obj, std::function<void(QObject *)> f)
{
  for (auto child : obj->children()) {
    f(child);
    withAllChildren(child, f);
  }
}

Quadro::Direction
Quadro::direction(const QPoint & abspos, const QRect & rect)
{
  QPoint center = QPoint(rect.width() / 2, rect.height() / 2);

  // transpose relative to widget center
  // necessary to get the proper quadrants in the unit circle
  QPoint relpos = QPoint(abspos.x() - center.x(), center.y() - abspos.y());

  // fix aspect ratio for rects where width != height
  relpos.setY(relpos.y() * (rect.width() / (double)rect.height()));

  double angle = std::atan(relpos.y() / (double)relpos.x());

  // 1. quadrant
  if (relpos.x() > 0 && relpos.y() > 0) {
    angle += 0.0;
  // 2. quadrant
  } else if (relpos.x() < 0 && relpos.y() > 0) {
    angle += M_PI;
  // 3. quadrant
  } else if (relpos.x() < 0 && relpos.y() < 0) {
    angle += M_PI;
  // 4. quadrant
  } else if (relpos.x() > 0 && relpos.y() < 0) {
    angle += 2 * M_PI;
  }

  // const double corner  = 0.01 * M_PI;
  const double corner  = 5.0 * (M_PI / 180.0);
  const double rad_000 = 0;
  const double rad_045 = M_PI_4;
  const double rad_135 = M_PI_2 + M_PI_4;
  const double rad_225 = M_PI + M_PI_4;
  const double rad_315 = M_PI + M_PI_2 + M_PI_4;
  const double rad_360 = 2 * M_PI;

  bool right  = (angle >= rad_000 && angle <= rad_045)
             || (angle >= rad_315 && angle <= rad_360);
  bool left   =  angle >= rad_135 && angle <= rad_225;
  bool top    =  angle >= rad_045 && angle <= rad_135;
  bool bottom =  angle >= rad_225 && angle <= rad_315;

  bool top_right    = angle > rad_045 - corner && angle < rad_045 + corner;
  bool top_left     = angle > rad_135 - corner && angle < rad_135 + corner;
  bool bottom_left  = angle > rad_225 - corner && angle < rad_225 + corner;
  bool bottom_right = angle > rad_315 - corner && angle < rad_315 + corner;

  Direction d = None;

  if (top_right) {
    d = TopRight;
  } else if (top_left) {
    d = TopLeft;
  } else if (bottom_right) {
    d = BottomRight;
  } else if (bottom_left) {
    d = BottomLeft;
  } else if (left) {
    d = Left;
  } else if (right) {
    d = Right;
  } else if (top) {
    d = Top;
  } else if (bottom) {
    d = Bottom;
  }

  return d;
}

Qt::CursorShape
Quadro::cursorShape(Direction d)
{
  switch (d) {
    case Left:
    case Right:
      return Qt::SizeHorCursor;
    case Top:
    case Bottom:
      return Qt::SizeVerCursor;
    case TopLeft:
    case BottomRight:
      return Qt::SizeFDiagCursor;
    case TopRight:
    case BottomLeft:
      return Qt::SizeBDiagCursor;
    case None:
    default:
      return Qt::WhatsThisCursor;
  };
}
Qt::CursorShape
Quadro::cursorShape(const QPoint & abspos, const QRect & rect)
{
  return cursorShape(direction(abspos, rect));
}

#endif // _QUADRO_UTIL_CPP
