/**
 * @file EventEmitter.h
 * @author MeerkatBoss (solodovnikov.ia@phystech.edu)
 *
 * @brief
 *
 * @version 0.1
 * @date 2023-11-22
 *
 * @copyright Copyright MeerkatBoss (c) 2023
 */
#ifndef __EVENT_EVENT_EMITTER_H
#define __EVENT_EVENT_EMITTER_H

#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Window.hpp>

#include "Plug/Event.h"
#include "Plug/Math.h"
#include "Plug/Widget.h"

class EventEmitter
{
public:
  EventEmitter(sf::Window& window, plug::TransformStack& stack) :
      m_window(window),
      m_stack(stack),
      m_shiftPressed(false),
      m_ctrlPressed(false),
      m_altPressed(false)
  {
  }

  EventEmitter(const EventEmitter&) = delete;
  EventEmitter& operator=(const EventEmitter&) = delete;

  ~EventEmitter(void) = default;

  void sendEvents(plug::Widget* widget);

private:
  plug::MouseMoveEvent convertMove(const sf::Event& sf_event) const;

  plug::MousePressedEvent convertMousePressed(const sf::Event& sf_event) const;

  plug::MouseReleasedEvent convertMouseReleased(
      const sf::Event& sf_event) const;

  plug::KeyboardPressedEvent convertKeyPressed(const sf::Event& sf_event) const;

  plug::KeyboardReleasedEvent convertKeyReleased(
      const sf::Event& sf_event) const;

  plug::TickEvent makeTickEvent(void);

  sf::Window&           m_window;
  plug::TransformStack& m_stack;
  sf::Clock             m_clock;

  bool m_shiftPressed;
  bool m_ctrlPressed;
  bool m_altPressed;
};

#endif /* EventEmitter.h */
