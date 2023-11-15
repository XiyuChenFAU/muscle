#!/bin/bash

outputs=(,"output_elbow_model_0p1" "output_elbow_model_0p01" "output_elbow_model_0p02" "output_elbow_model_0p05","output_elbow_model_origin" "output_elbow_model_y_0p05_collision" "output_elbow_model_y_0p05_collision_minimize" "output_elbow_model_y_0p05_collision_minimize_mass","output_elbow_model_y_0p05_collision_minimize_mass_length" "output_elbow_model_z_180" "output_elbow_model_z_360" "output_elbow_model_z_720","output_elbow_model_z_1080" "output_elbow_model_z_1080_minimize" "output_elbow_model_z_1080_minimize_mass" "output_elbow_model_z_1080_minimize_mass_length")


for output in "${outputs[@]}"; do
    model_name="${output#output_}"

    cd "$output" || exit 1

    python3 plot.py -model "$model_name" -scale 0.23 -viewangle -90

    wait

    cd ..
done
