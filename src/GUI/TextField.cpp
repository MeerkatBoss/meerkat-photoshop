#include "GUI/TextField.h"
#include <cstring>
#include "GUI/Widget.h"
#include "Layout/LayoutBox.h"
#include "Sprite/TextSprite.h"

namespace gui
{

TextField::TextField(const char* header, const char* default_text,
          const plug::LayoutBox& layout_box) :
  Widget(layout_box),
  m_titlebar(header, layout::LayoutBox(100_per, 1_cm, layout::Align::TopCenter)),
  m_background(plug::Color(100, 100, 100), 10),
  m_fieldLayoutBox(100_per, 100_per, layout::Align::BottomCenter),
  m_fieldBg(plug::Color(0, 0, 0), 7),
  m_fieldText(default_text, 60, plug::Color(255, 255, 255), TextAlign::Left),
  m_textLen(strnlen(default_text, MAX_TEXT_LEN))
{
  strncpy(m_text, default_text, MAX_TEXT_LEN);
  m_text[MAX_TEXT_LEN] = '\0';

  m_fieldLayoutBox.setPadding(2_cm, 1_cm, .5_cm, .5_cm);
}

void TextField::draw(plug::TransformStack& stack,
                  plug::RenderTarget&   target)
{
  m_background.draw(getLayoutBox(), stack, target);
  stack.enter(Transform(getLayoutBox().getPosition()));
  m_titlebar.draw(stack, target);
  m_fieldBg.draw(m_fieldLayoutBox, stack, target);
  m_fieldText.draw(m_fieldLayoutBox, stack, target);
  stack.leave();
}

void TextField::onParentUpdate(const plug::LayoutBox& parent_box)
{
  Widget::onParentUpdate(parent_box);
  m_titlebar.onParentUpdate(getLayoutBox());
  m_fieldLayoutBox.onParentUpdate(getLayoutBox());
}

static char getCharFromEvent(const plug::KeyboardPressedEvent& event);

void TextField::onKeyboardPressed(const plug::KeyboardPressedEvent& event,
                               plug::EHC& context)
{
  if (context.stopped)
  {
    return;
  }
  if (event.key_id == plug::KeyCode::Backspace)
  {
    if (m_textLen > 0)
    {
      --m_textLen;
      m_text[m_textLen] = '\0';
      m_fieldText.setText(m_text);
    }
  }
  else if (m_textLen < MAX_TEXT_LEN)
  {
    const char appended = getCharFromEvent(event);
    if (appended != '\0')
    {
      m_text[m_textLen] = appended;
      ++m_textLen;
      m_text[m_textLen] = '\0';
      m_fieldText.setText(m_text);
    }
  }
}

static char getCharFromEvent(const plug::KeyboardPressedEvent& event)
{
#define CAPTURE_SYMBOL(code, plain, shifted)\
  case plug::KeyCode::code: return event.shift ? (shifted) : (plain)

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wswitch-enum"
  switch (event.key_id)
  {
  CAPTURE_SYMBOL(A, 'a', 'A');
  CAPTURE_SYMBOL(B, 'b', 'B');
  CAPTURE_SYMBOL(C, 'c', 'C');
  CAPTURE_SYMBOL(D, 'd', 'D');
  CAPTURE_SYMBOL(E, 'e', 'E');
  CAPTURE_SYMBOL(F, 'f', 'F');
  CAPTURE_SYMBOL(G, 'g', 'G');
  CAPTURE_SYMBOL(H, 'h', 'H');
  CAPTURE_SYMBOL(I, 'i', 'I');
  CAPTURE_SYMBOL(J, 'j', 'J');
  CAPTURE_SYMBOL(K, 'k', 'K');
  CAPTURE_SYMBOL(L, 'l', 'L');
  CAPTURE_SYMBOL(M, 'm', 'M');
  CAPTURE_SYMBOL(N, 'n', 'N');
  CAPTURE_SYMBOL(O, 'o', 'O');
  CAPTURE_SYMBOL(P, 'p', 'P');
  CAPTURE_SYMBOL(Q, 'q', 'Q');
  CAPTURE_SYMBOL(R, 'r', 'R');
  CAPTURE_SYMBOL(S, 's', 'S');
  CAPTURE_SYMBOL(T, 't', 'T');
  CAPTURE_SYMBOL(U, 'u', 'U');
  CAPTURE_SYMBOL(V, 'v', 'V');
  CAPTURE_SYMBOL(W, 'w', 'W');
  CAPTURE_SYMBOL(X, 'x', 'X');
  CAPTURE_SYMBOL(Y, 'y', 'Y');
  CAPTURE_SYMBOL(Z, 'z', 'Z');
  CAPTURE_SYMBOL(Num0, '0', ')');
  CAPTURE_SYMBOL(Num1, '1', '!');
  CAPTURE_SYMBOL(Num2, '2', '@');
  CAPTURE_SYMBOL(Num3, '3', '#');
  CAPTURE_SYMBOL(Num4, '4', '$');
  CAPTURE_SYMBOL(Num5, '5', '%');
  CAPTURE_SYMBOL(Num6, '6', '^');
  CAPTURE_SYMBOL(Num7, '7', '&');
  CAPTURE_SYMBOL(Num8, '8', '*');
  CAPTURE_SYMBOL(Num9, '9', '(');
  CAPTURE_SYMBOL(Comma, ',', '<');
  CAPTURE_SYMBOL(Period, '.', '>');
  CAPTURE_SYMBOL(Hyphen, '-', '_');
  CAPTURE_SYMBOL(Space, ' ', ' ');
  default:
    break;
  }
#pragma clang diagnostic pop
#undef CAPTURE_SYMBOL

  return '\0';
}

} // namespace gui
