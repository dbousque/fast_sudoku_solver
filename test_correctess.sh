#./hard_sudokus_this_version.sh > this_version_results
./solver_smart_test > this_version_smart_results
#diff this_version_results tdoku/correct_results
diff this_version_smart_results tdoku/correct_results