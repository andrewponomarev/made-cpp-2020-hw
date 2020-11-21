#ifndef MEANWHILE_IN_THE_SEVENTH_GALAXY_FIRST_VARIANT_GAME_H
#define MEANWHILE_IN_THE_SEVENTH_GALAXY_FIRST_VARIANT_GAME_H

#include "xor_sum.h"
#include "binary.h"


template<size_t xor_sum, size_t Head, size_t... Tail>
struct GetFirstMove {
  static constexpr size_t head_xor_sum = XorSum<xor_sum, Head>::value;
  static constexpr size_t tail_xor_sum = GetFirstMove<xor_sum, Tail...>::min_xor_sum;
  static constexpr bool cond = head_xor_sum < tail_xor_sum;
  static constexpr size_t min_xor_sum = cond * head_xor_sum + !cond * tail_xor_sum;
  static constexpr size_t heap_index = !cond * (GetFirstMove<xor_sum, Tail...>::heap_index + 1);
  static constexpr size_t rocks_count = cond * (Head - min_xor_sum) + !cond * GetFirstMove<xor_sum, Tail...>::rocks_count;
};

template<size_t xor_sum, size_t Head>
struct GetFirstMove<xor_sum, Head> {
  static constexpr size_t min_xor_sum = XorSum<xor_sum, Head>::value;
  static constexpr size_t heap_index = 0;
  static constexpr size_t rocks_count = Head - min_xor_sum;
};


template <size_t... Heaps>
struct Game {
  static constexpr size_t xor_sum = XorSum<Heaps...>::value;
  static constexpr bool first_player_wins = xor_sum ;
  static constexpr size_t first_move_heap_index = first_player_wins * GetFirstMove<xor_sum, Heaps...>::heap_index + (!first_player_wins) * (-1);
  static constexpr ssize_t first_move_rocks_count = first_player_wins * GetFirstMove<xor_sum, Heaps...>::rocks_count + (!first_player_wins) * (-1);
};


#endif /// MEANWHILE_IN_THE_SEVENTH_GALAXY_FIRST_VARIANT_GAME_H.
