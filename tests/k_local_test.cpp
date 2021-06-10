// include Google Test
#include <gtest/gtest.h>
#include <gmock/gmock.h>
// include STL
#include <iostream>
#include <utility>
#include <numeric>
#include <random>
#include <tuple>
#include <type_traits>
#include <vector>
#include <algorithm>
#include <chrono>

// include OpenJij
#include <graph/all.hpp>
#include <system/all.hpp>
#include <updater/all.hpp>
#include <algorithm/all.hpp>
#include <result/all.hpp>
#include <utility/schedule_list.hpp>
#include <utility/union_find.hpp>
#include <utility/random.hpp>
#include <utility/gpu/memory.hpp>
#include <utility/gpu/cublas.hpp>

static openjij::utility::ClassicalScheduleList generate_schedule_list(double beta, int mc_step) {
   auto list = openjij::utility::ClassicalScheduleList(1);
   list[0].one_mc_step = mc_step;
   list[0].updater_parameter = openjij::utility::ClassicalUpdaterParameter(beta);
   return list;
}

TEST(KLocal, test0) {
   
   openjij::graph::Index num_spins = 3;
   openjij::graph::Polynomial<double> poly_graph(num_spins, cimod::Vartype::BINARY);
   
   poly_graph.J(   {0}   ) = -1;//0
   poly_graph.J( {0, 1}  ) = -1;//1
   poly_graph.J( {0, 2}  ) = -1;//2
   poly_graph.J({0, 1, 2}) = -1;//3
   poly_graph.J(   {1}   ) = -1;//4
   poly_graph.J( {1, 2}  ) = -1;//5
   poly_graph.J(   {2}   ) = -1;//6
   
   openjij::graph::Spins spin = {0, 0, 1};
   
   auto poly_system = openjij::system::make_k_local_polynomial(spin, poly_graph);
   
//   poly_system.PrintInfo();
   
   const int seed = 1;
   
   auto random_numder_engine = std::mt19937(seed);
   const auto schedule_list = generate_schedule_list(100, 1);
   poly_system.print_adj();
   poly_system.print_dE();
   poly_system.print_zero_count();
   openjij::algorithm::Algorithm<openjij::updater::KLocal>::run(poly_system, random_numder_engine, schedule_list);
   poly_system.print_dE();
   poly_system.print_zero_count();
   //openjij::algorithm::Algorithm<openjij::updater::KLocal>::run(poly_system, random_numder_engine, schedule_list);

   //poly_system.PrintInfo();
   
   const auto result_spin_poly = openjij::result::get_solution(poly_system);
   for (std::size_t i = 0; i < result_spin_poly.size(); ++i) {
      printf("Result_spin[%ld]=%d\n", i , result_spin_poly[i]);
   }
   
   
}
/*
TEST(KLocal, test1) {
   
   openjij::graph::Index num_spins = 10;
   openjij::graph::Polynomial<double> poly_graph(num_spins, cimod::Vartype::BINARY);
   
   poly_graph.J({0,1,2,3,4,5,6,7,8,9}) = -1;
   
   openjij::graph::Spins spin = {1, 0, 1, 0, 1, 0, 1, 0, 1, 0};
   
   auto poly_system = openjij::system::make_k_local_polynomial(spin, poly_graph);
   
   //poly_system.PrintInfo();
   
   const int seed = 1;
   
   auto random_numder_engine = std::mt19937(seed);
   const auto schedule_list = generate_schedule_list(100, 1);
   
   openjij::algorithm::Algorithm<openjij::updater::KLocal>::run(poly_system, random_numder_engine, schedule_list);
   
   //openjij::algorithm::Algorithm<openjij::updater::KLocal>::run(poly_system, random_numder_engine, schedule_list);

   //poly_system.PrintInfo();
   
   const auto result_spin_poly = openjij::result::get_solution(poly_system);
   for (std::size_t i = 0; i < result_spin_poly.size(); ++i) {
      printf("Result_spin[%ld]=%d\n", i , result_spin_poly[i]);
   }
   
}

TEST(KLocal, test2) {
   
   openjij::graph::Index num_spins = 3;
   openjij::graph::Polynomial<double> poly_graph(num_spins, cimod::Vartype::BINARY);
   
   poly_graph.J({0,1,2}) = +1;
   //poly_graph.J({0,2}) = -2;

   
   openjij::graph::Spins spin = {0, 0, 0};
   
   auto poly_system = openjij::system::make_k_local_polynomial(spin, poly_graph);
   
   //poly_system.PrintInfo();
   
   const int seed = 1;
   
   auto random_numder_engine = std::mt19937(seed);
   const auto schedule_list = generate_schedule_list(100, 10);
   
   openjij::algorithm::Algorithm<openjij::updater::KLocal>::run(poly_system, random_numder_engine, schedule_list);
   
   //openjij::algorithm::Algorithm<openjij::updater::KLocal>::run(poly_system, random_numder_engine, schedule_list);

   //poly_system.PrintInfo();
   
   const auto result_spin_poly = openjij::result::get_solution(poly_system);
   for (std::size_t i = 0; i < result_spin_poly.size(); ++i) {
      printf("Result_spin[%ld]=%d\n", i , result_spin_poly[i]);
   }
   
}
*/
