/* A simple toolkit to help beginners using the <random> library an easier task
 *
 * C++20 module interface file */

 // v1.3.1.2

  // shamelessly stolen and adapted from a C++ working paper: WG21 N3551
  // http://www.open-std.org/JTC1/SC22/WG21/docs/papers/2013/n3551.pdf

export module random_toolkit;

// stop the WinAPI MIN/MAX macros from being used
// instead of C++ std::/min/std::max
// use this define where <windows.h> is included
// #define NOMINMAX

import <chrono>;
import <random>;
import <stdexcept>;
import <limits>;

namespace rtk
{
   static bool seeded { false };

   export std::mt19937& urng( )
   {
      static std::mt19937 URNG { };

      return URNG;
   }

   // function to initialize (seed) the PRNG engine
   // uses a seed sequence for better randomization
   export void srand(bool FORCE_SEED = false)
   {
      static const std::seed_seq::result_type seeds[ ] { std::random_device {} (),
                                                         std::seed_seq::result_type(std::chrono::system_clock::now( ).time_since_epoch( ).count( )) };

      static std::seed_seq sseq(std::begin(seeds), std::end(seeds));

      // the URNG can't be reseeded unless forced
      if ( !seeded || FORCE_SEED )
      {
         urng( ).seed(sseq);

         seeded = true;
      }
   }

   // function to initialize the PRNG engine
   // using a specified seed
   export void srand(signed seed, bool FORCE_SEED = false)
   {
      // the URNG can't be reseeded unless forced
      if ( !seeded || FORCE_SEED )
      {
         urng( ).seed(static_cast<unsigned>(seed));

         seeded = true;
      }
   }

   // two functions to obtain uniform distribution ints and doubles
   export int rand_int(int from, int to)
   {
      static std::uniform_int_distribution<> dist { };

      if ( from > to ) { throw std::invalid_argument("bad int distribution params"); }

      return dist(urng( ), decltype(dist)::param_type { from, to });
   }

   export double rand_rl(double from, double to)
   {
      static std::uniform_real_distribution<> dist { };

      // a real distribution kinda goes flakey when the params are equal, divide by zero will do that,
      // as well as reversed from expected
      if ( !(from < to && ((to - from) <= std::numeric_limits<double>::max( ))) )
      {
         throw std::invalid_argument("bad double distribution params");
      }

      return dist(urng( ), decltype(dist)::param_type { from, to });
   }

   // two function overloads to obtain uniform distribution ints and doubles
   export int rand(int from, int to)
   {
      return rand_int(from, to);
   }

   export double rand(double from, double to)
   {
      return rand_rl(from, to);
   }

   // function for rolling dice, and checking if the # of pips is nonstandard
   export int roll_die(int pips)
   {
      // check to see if the number of die pips is less than 2
      if ( pips < 2 )
      {
         return -1;
      }

      return rand(1, pips);
   }
}