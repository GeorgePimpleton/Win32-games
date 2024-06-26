﻿// C++20

import <array>;
import <iostream>;
import <numeric>;
import <algorithm>;

import random_toolkit;

int main( )
{
   rtk::srand( );

   using card = unsigned;

   std::array<card, 52> deck { };

   std::iota(deck.begin( ), deck.end( ), 0);

   auto rank = [ ] (card c) { return "AKQJT98765432"[ c % 13 ]; };
   auto suit = [ ] (card c) { return "SHDC"[ c / 13 ]; };

   for ( card count { }; card c : deck )
   {
      std::cout << rank(c) << suit(c) << ' ';
      count++;

      if ( 0 == (count % 13) ) { std::cout << '\n'; }
   }
   std::cout << '\n';

   std::shuffle(deck.begin( ), deck.end( ), rtk::urng( ));

   for ( card count { }; card c : deck )
   {
      std::cout << rank(c) << suit(c) << ' ';
      count++;

      if ( 0 == (count % 13) ) { std::cout << '\n'; }
   }
   std::cout << '\n';

   for ( unsigned loop { }; loop < 50; loop++ )
   {
      std::cout << rtk::roll_die(6) << ' ';

      if ( (loop + 1) % 20 == 0 ) { std::cout << '\n'; }
   }
   std::cout << "\n\n";

   auto flip_coin = [ ] ( ) { return (rtk::rand(0, 1) ? "Heads" : "Tails"); };

   for ( size_t loop { }; loop < 25; loop++ )
   {
      std::cout << flip_coin( ) << '\t';

      if ( (loop + 1) % 8 == 0 ) { std::cout << '\n'; }
   }
   std::cout << "\n\n";

   std::cout << "Creating a bad random distribution, it should be be\n";
   std::cout << "encapsulated within a try/catch block.  Unless you want to crash.\n";

   try
   {
      int itest1 { rtk::rand(5, 2) };
   }
   catch ( const std::exception& e )
   {
      std::cerr << "\n>>> A standard exception was caught, with message '" << e.what( ) << "'\n";
   }

   try
   {
      int itest2 { rtk::rand_int(5, 2) };
   }
   catch ( const std::exception& e )
   {
      std::cerr << "\n>>> A standard exception was caught, with message '" << e.what( ) << "'\n";
   }

   try
   {
      double dtest1 { rtk::rand(5.3, 2.8) };
   }
   catch ( const std::exception& e )
   {
      std::cerr << "\n>>> A standard exception was caught, with message '" << e.what( ) << "'\n";
   }

   try
   {
      double dtest2 { rtk::rand_rl(5.3, 2.8) };
   }
   catch ( const std::exception& e )
   {
      std::cerr << "\n>>> A standard exception was caught, with message '" << e.what( ) << "'\n";
   }

   try
   {
      double dtest3 { rtk::rand(5.3, 5.3) };
   }
   catch ( const std::exception& e )
   {
      std::cerr << "\n>>> A standard exception was caught, with message '" << e.what( ) << "'\n";
   }

   std::cout << "\nLet's see if we can have a non-standard die.....\nA die with 1 side: ";
   std::cout << rtk::roll_die(1) << '\n';

   std::cout << "A die with zero sides: ";
   std::cout << rtk::roll_die(0) << '\n';

   std::cout << "A die with negative sides: ";
   std::cout << rtk::roll_die(-6) << '\n';
}
