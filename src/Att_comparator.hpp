#pragma once

#include <Automaton.h>

#define TINYMACHINE
#undef FACTORY
#undef STATE_TYPE
#undef MACHINE

#ifdef TINYMACHINE
#define MACHINE TinyMachine
#define FACTORY 0
#define STATE_TYPE tiny_state_t
#else
#define MACHINE Machine
#define FACTORY factory
#define STATE_TYPE state_t
#endif

typedef bool ( *atm_comparator_cb_t )( int idx, int v, int up, int idx_threshold, int v_threshold );

class Att_comparator : public MACHINE {
 public:
  Att_comparator( void ) : MACHINE() {
#ifndef TINYMACHINE
    class_label = "CMP";
#endif
  };

  short pin;
  atm_timer_millis timer;
  int v_sample, v_threshold, v_previous;
  uint64_t bitmap_sample, bitmap_previous, bitmap_diff;
  uint16_t* p_threshold;  // Max 64 values
  uint16_t p_threshold_size;

  uint16_t* avg_buf;
  uint16_t avg_buf_size;
  uint16_t avg_buf_head;
  uint32_t avg_buf_total;
  atm_connector _onup, _ondown;

  enum { IDLE, SAMPLE, SEND } STATES;
  enum { EVT_TRIGGER, EVT_TIMER, ELSE } EVENTS;
  enum { ACT_SAMPLE, ACT_SEND } ACTIONS;

  Att_comparator& begin( int attached_pin, int sampleRate = 50 );
  Att_comparator& threshold( uint16_t* v, uint16_t size, bool catchUp = false );
  Att_comparator& average( uint16_t* v, uint16_t size );
  Att_comparator& onUp( atm_comparator_cb_t callback, int idx = 0 );
  Att_comparator& onUp( Machine& machine, int event = 0 );
  Att_comparator& onUp( TinyMachine& machine, int event = 0 );
  Att_comparator& onUp( const char* label, int event = 0 );
  Att_comparator& onDown( atm_comparator_cb_t callback, int idx = 0 );
  Att_comparator& onDown( Machine& machine, int event = 0 );
  Att_comparator& onDown( TinyMachine& machine, int event = 0 );
  Att_comparator& onDown( const char* label, int event = 0 );
  int _avg();
  Att_comparator& bitmap( uint16_t v );
  int state( void );
  int sample();
  virtual int read_sample();
  int event( int id );
  void action( int id );
  Att_comparator& trace( Stream& stream );
};