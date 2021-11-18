# Launch file functions
bringup_name=test_pepper_desktop

package_path=`rospack find ${bringup_name}_bringup`
launch_files=$(find ${package_path}/launch -name '*.launch')
for launch_file in ${launch_files}; do
    launch_file_name=$(echo ${launch_file##*/} | cut -d"." -f1)
    eval "${bringup_name//_/-}-${launch_file_name//_/-}() { roslaunch ${launch_file} \$@; }"
done

