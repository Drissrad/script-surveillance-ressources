#!/bin/bash

LOG_FOLDER=$(pwd)

display_help() {
    echo "Usage: $0 [-d <start_date>] [-f] [-r <resources>] [-k] [-h] [-t] [-l <log_folder>] [-v]"
    echo "-d : Specify start date."
    echo "-e : Specify end date."
    echo "-r : Specify the resources to track (disk, ram, cpu). Default is all of them."
    echo "-k : Kill all program current processes."
    echo "-h : Display program documentation."
    echo "-l : Specify the folder to store the logs of each resource."
    echo "-v : Visualize the recorded data using a graph on gnuplot."
}

while getopts ":d:e:fr:kt:l:hv" opt; do
    case $opt in
        d) START_DATE=$OPTARG ;;
        e) END_DATE=$OPTARG ;;
        r) RESOURCE_TRACKER=true
           RESOURCES=$OPTARG ;;
        k) KILL=true ;;
        l) LOG_FOLDER=$OPTARG ;;
        v) VISUALIZE=true ;;
        h) display_help
           exit 0 ;;
        \?) echo "Invalid option: -$OPTARG" >&2
            exit 1 ;;
    esac
done


install_gnuplot() {
    if ! command -v gnuplot &> /dev/null; then
        echo "Installing gnuplot..."
        sudo apt-get update
        sudo apt-get install -y gnuplot
    fi
}

change_directory() {
    if [ -n "$LOG_FOLDER" ]; then
        mkdir -p "$LOG_FOLDER"
        cd "$LOG_FOLDER" || exit 1
    fi
}

kill_program_processes() {
    pkill -f "$(basename "$0")"
}

if [ -z "$RESOURCES" ]; then
    RESOURCES="cpu,ram,disk"
fi

if [ -z "$NOHUP" ]; then
    NOHUP="yes" nohup "$0" "$@" > /dev/null 2>&1 &
    exit
fi

SLEEP_TIME=3
SCRIPT_PATH=$(realpath "$0")
echo $SCRIPT_PATH
monitor_cpu() {
echo "writing to $LOG_FOLDER"
    while true; do
        cpu_usage=$(top -bn1 | grep 'Cpu(s)' | awk '{print $2}')
        echo "$(date +'%Y-%m-%d %H:%M:%S'),$cpu_usage" >> "$LOG_FOLDER/cpu_usage_$(date +'%Y%m%d').txt"
        sleep $SLEEP_TIME
    done
}
 
monitor_ram() {
    echo "Writing to $LOG_FOLDER"
    while true; do
        RAM_INFO=$(free -h | grep 'Mem')
        USED_RAM=$(echo "$RAM_INFO" | awk '{print $3}')
        echo "$(date +'%Y-%m-%d %H:%M:%S'),$USED_RAM" >> "$LOG_FOLDER/ram_usage_$(date +'%Y%m%d').txt"
        sleep $SLEEP_TIME
    done
}
 
monitor_disk() {
echo "Writing to $LOG_FOLDER"
    while true; do
        DISK_INFO=$(df -hP /)
        USED_DISK=$(echo "$DISK_INFO" | awk '/\/$/ && NR>1 {print $3}')
        echo "$(date +'%Y-%m-%d %H:%M:%S'),$USED_DISK" >> "$LOG_FOLDER/disk_usage_$(date +'%Y%m%d').txt"
        sleep $SLEEP_TIME
    done
}
visualize_data() {
    echo "Visualizing data..."
    if [ -n "$VISUALIZE" ]; then
        if [ -z "$START_DATE" ]; then
            for resource in $(echo "$RESOURCES" | tr ',' ' '); do
                ./usage_plot "$resource"
            done
        else
            for resource in $(echo "$RESOURCES" | tr ',' ' '); do
                ./usage_plot "$resource" "$START_DATE" "$END_DATE"
            done
        fi
    fi
    echo "End of visualization"
}



add_to_startup() {
    if ! crontab -l | grep -qF "$SCRIPT_PATH"; then
        (crontab -l ; echo "@reboot nohup $SCRIPT_PATH") | crontab -
    fi
}

main() {
    echo "Starting monitoring system..."
    if [ "$KILL" ]; then
        kill_program_processes
        exit 0
    fi
        echo "$VISUALIZE"

    if [ "$VISUALIZE" ]; then
        visualize_data
        exit 0
    fi
	
	monitor_cpu &
	monitor_ram &
	monitor_disk &
		
    add_to_startup
    wait
}

main
