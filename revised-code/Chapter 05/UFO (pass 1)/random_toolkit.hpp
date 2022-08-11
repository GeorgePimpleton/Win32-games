/* A simple toolkit to help beginners using the <random> library an easier task */

 // shamelessly stolen and adapted from a C++ working paper: WG21 N3551
 // http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2013/n3551.pdf

#pragma once

#include <chrono>
#include <random>
#include <stdexcept>

namespace rtk
{
   static bool seeded { false };

   inline std::mt19937& urng()
   {
      static std::mt19937 URNG { };

      return URNG;
   }

   inline void srand(bool FORCE_SEED = false)
   {
      static const std::seed_seq::result_type seeds[] { std::random_device {} (),
                                                        std::seed_seq::result_type(std::chrono::system_clock::now().time_since_epoch().count()) };

      static std::seed_seq sseq(std::begin(seeds), std::end(seeds));

      if ( !seeded || FORCE_SEED )
      {
         urng().seed(sseq);

         seeded = true;
      }
   }

   inline void srand(signed seed, bool FORCE_SEED = false)
   {
      if ( !seeded || FORCE_SEED )
      {
         urng().seed(static_cast<unsigned>(seed));

         seeded = true;
      }
   }

   inline int rand(int from, int to)
   {
      static std::uniform_int_distribution<> dist { };

      if ( from > to ) { throw std::invalid_argument("bad int distribution params"); }

      return dist(urng(), decltype(dist)::param_type { from, to });
   }

   inline double rand(double from, double to)
   {
      static std::uniform_real_distribution<> dist { };

      if ( from > to ) { throw std::invalid_argument("bad double distribution params"); }

      return dist(urng(), decltype(dist)::param_type { from, to });
   }

   inline int roll_die(int pips)
   {
      if ( pips < 2 )
      {
         return -1;
      }

      return rand(1, pips);
   }
}
