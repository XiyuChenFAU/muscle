#!/bin/bash

#outputs=("output_shoulder_ellipsoid_m100_100_global_y" "output_shoulder_ellipsoid_m100_100_global_y_minimize" "output_shoulder_ellipsoid_m100_100_global_y_minimize_length" "output_shoulder_ellipsoid_m100_100_global_y_minimize_mass" "output_shoulder_ellipsoid_m100_100_global_y_minimize_length_10" "output_shoulder_ellipsoid_m100_100_global_y_minimize_length_100" "output_shoulder_ellipsoid_m100_100_global_y_minimize_length_1000" "output_shoulder_ellipsoid_m100_100_global_y_minimize_length_10000" "output_shoulder_ellipsoid_m100_100_global_y_minimize_length_0p1" "output_shoulder_ellipsoid_m100_100_global_y_minimize_length_0p05" "output_shoulder_ellipsoid_m100_100_global_y_minimize_length_0p01")

outputs=("output_shoulder_ellipsoid_m100_100_global_y_minimize_length_1000_node_100")


for output in "${outputs[@]}"; do
    model_name="${output#output_}"

    cd "$output" || exit 1
    
        
    python3 plot_momenarm.py "$model_name"

    wait

    python3 plot.py -model "$model_name" -scale 0.09 -viewangle -90

    wait


    cd ..
done