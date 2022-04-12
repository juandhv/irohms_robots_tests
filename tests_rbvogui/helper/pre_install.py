#!/usr/bin/env python

from pathlib import Path
import os, yaml, requests

# some consts
REPO_LIST_URL = "https://raw.githubusercontent.com/RobotnikAutomation/rbvogui_sim/melodic-devel/repos/rbvogui_sim_devel.repos"
TARGETS_DIR = "~/ros/melodic/.env/targets/"
TARGET_FILE = "install.yaml"
TARGETS = [
  "rcomponent",
  "teleop_panel",
  "ewellix_description",
  "velodyne_simulator",
  "rg2_gripper_common",
  "roboticsgroup_gazebo_plugins",
  "joint_read_command_controller",
  "robotnik_msgs",
  "robotnik_sensors",
  "robotnik_pad",
]

META_TARGETS = {
  "rbvogui_common": [
    "rbvogui_control",
    "rbvogui_description",
    "rbvogui_localization",
    "rbvogui_navigation",
    "rbvogui_pad",
    "rbvogui_robot_local_control",
    "rbvogui_ur5_rg2_moveit"
  ],
  
  "universal_robot": [
    "ur_description",
    "ur5_moveit_config",
    "ur_gazebo"
  ], 
  
  "rbvogui_sim": [
    "rbvogui_gazebo",
    "rbvogui_sim_bringup"
  ]
}

# some utilities
def safetouch(folder_path, file=TARGET_FILE):
    folder_path = Path(folder_path).expanduser()
    file_path = folder_path / file
    
    if file not in os.listdir(folder_path):
        open(file_path, 'a').close()  # safe create, no overwrite, close on open
      
    return file_path

# get repos
repos = requests.get(REPO_LIST_URL).text
repos = yaml.safe_load(repos)
repos = repos["repositories"]

# get targets list
targets_dir = Path(TARGETS_DIR).expanduser()
if not targets_dir.exists():
    pass  # crash(f'Target directory not found at {TARGETS_DIR} or permission not granted to execute os.stat()')

targets = os.listdir(targets_dir)

# for each repo:
for repo, details in repos.items():
    repo = repo[4:]  # slice off 'src/' from start of string
    
    if repo not in TARGETS and repo not in META_TARGETS:
        continue
    
    if repo in META_TARGETS:
        for rp in META_TARGETS[repo]:
            # create or update target TODO: create func
            ros_repo = "ros-"+rp
            repo_dir = targets_dir / ros_repo
            
            # verify repo pkg in targets: otherwise create TODO: skip create???
            if ros_repo not in targets:
                os.makedirs(repo_dir)
                print("mkdir success: ", repo_dir)
            
            # verify install.yaml in repo pkg: otherwise create
            print("repo_dir :", repo_dir)  # TODO: restructure to "Found repo at ~/... !"
            target_file = safetouch(file=TARGET_FILE, folder_path=repo_dir)
            
            # read target yaml
            target_yaml = yaml.safe_load(target_file.open())
            
            repo_url = details["url"].replace("https://", "git@").replace('/', ':', 1)
            if repo_url[-4:] != ".git":
                repo_url = repo_url+".git"
            print("url: ", repo_url)
            
            if target_yaml is None:
                print("blank yaml, attempting update")
                target_yaml = [{
                   "source": {
                     "type": details["type"],
                     "url": repo_url,
                     "version": details["version"],
                   },
                   
                   "type": "ros"
                }]
                
            else:  # target yaml is not empty
                print(target_yaml[0])  # TODO: remove
                # check git target: otherwise skip
                if target_yaml[0]['source']['type'] == 'git':
                    # verify url and version match: otherwise update
                    target_yaml[0]['source']['url'] = repo_url
                    target_yaml[0]['source']['version'] = details['version']
            
            # write yaml to target file
            yaml.safe_dump(target_yaml, target_file.open('w'))
        
        continue # move to next repo
        
    
    ros_repo = "ros-"+repo
    repo_dir = targets_dir / ros_repo
    
    # verify repo pkg in targets: otherwise create
    if ros_repo not in targets:
        os.makedirs(repo_dir)
        print("mkdir success: ", repo_dir)  # TODO: restructure
    
    # verify install.yaml in repo pkg: otherwise create
    print("repo_dir :", repo_dir)
    target_file = safetouch(file=TARGET_FILE, folder_path=repo_dir)
    
    # read target yaml
    target_yaml = yaml.safe_load(target_file.open())
    print(target_yaml[0])  # TODO: remove
    
    repo_url = details["url"].replace("https://", "git@").replace('/', ':', 1)
    if repo_url[-4:] != ".git":
        repo_url = repo_url+".git"
        
    # ensure target yaml not blank: otherwise fill out template and write file
    if target_yaml is None:
        target_yaml = [{
           "source": {
             "type": details["type"],
             "url": repo_url,
             "version": details["version"],
           },
           
           "type": "ros"
        }]
    
    else: 
        # check git target: otherwise skip
        if target_yaml[0]['source']['type'] == 'git':
            # verify url and version match: otherwise update
            target_yaml[0]['source']['url'] = repo_url
            target_yaml[0]['source']['version'] = details['version']
    
    # write yaml to target file
    yaml.safe_dump(target_yaml, target_file.open('w'))


