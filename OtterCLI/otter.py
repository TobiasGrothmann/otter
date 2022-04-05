#!/usr/bin/python3
import os
import sys
from pathlib import Path
import shutil

# print to stderr
def eprint(*args, **kwargs):
    print(*args, file=sys.stderr, **kwargs)

# SETUP
class STYLE:
	HEADER = '\033[95m'
	OKBLUE = '\033[94m'
	OKCYAN = '\033[96m'
	OKGREEN = '\033[92m'
	WARNING = '\033[93m'
	FAIL = '\033[91m'
	ENDC = '\033[0m'
	BOLD = '\033[1m'
	UNDERLINE = '\033[4m'
def style(msg, *styles):
	return "%s%s%s" % ("".join(styles), msg, STYLE.ENDC)

def styleError(msg):
	return style(msg, STYLE.FAIL)
def error(msg):
	eprint(styleError("ERROR:"), msg)
def styleSuccess(msg):
	return style(msg, STYLE.OKGREEN)
def success(msg):
	print(styleSuccess("DONE:"), msg)
def abort():
	exit(1)

def printHelp():
	eprint(style("usage:\totter [command] [args]\n", STYLE.FAIL, STYLE.BOLD))
	# create
	eprint(style("create <path>", STYLE.OKGREEN, STYLE.BOLD))
	eprint("\tcreates a new otter project at " + style("<path>", STYLE.OKGREEN))
	eprint(style("\totter create ~/OtterProjects/newProject\n", STYLE.OKCYAN))
	# reset
	eprint(style("reset <path>", STYLE.OKGREEN, STYLE.BOLD))
	eprint("\tresets the xcode project at " + style("<path>", STYLE.OKGREEN))
	eprint(style("\totter reset ~/OtterProjects/newProject", STYLE.OKCYAN))


# CHECK OTTER_HOME
OTTER_HOME = os.getenv('OTTER_HOME')
SAMPLE_PROJECT_NAME = "OtterSampleProject"
def getOtterSampleProjectPath():
	return home / "OtterCLI" / SAMPLE_PROJECT_NAME
def getOtterSampleProjectXcodePath():
	return home / "OtterCLI" / SAMPLE_PROJECT_NAME / (SAMPLE_PROJECT_NAME+".xcodeproj")

if not OTTER_HOME:
	error("OTTER_HOME is not set. Something went wrong with your installation.")
	abort()
home = Path(OTTER_HOME)
if not home.is_dir():
	error(f"OTTER_HOME is set to '{home}' which is not a directory")
	abort()


# PARSE ARGUMENTS
arguments = sys.argv[1:]
if len(arguments) == 0:
	printHelp()
	abort()

command = arguments[0]
if (command.replace("-", "") in ("help", "h")):
	printHelp()
	abort()



# RENAMING
def replaceFileContents(filePath, sourceString, targetString):
	fileContent = ""
	with open(filePath, "r") as fileRead:
		fileContent = fileRead.read()
	fileContent = fileContent.replace(sourceString, targetString)
	with open(filePath, "wt") as fileWrite:
		fileWrite.write(fileContent)



# ACTIONS
class Action():
	def __init__(self):
		self.valid = False

class Create(Action):
	def __init__(self, arguments):
		super().__init__()
		if (len(arguments) != 1):
			error("Create requires argument: " + styleError("<path>"))
			return
		self.targetPath = Path(arguments[0])
		if self.targetPath.is_dir():
			error(f"Directory '{styleError(self.targetPath)}' already exists.")
			return
		if self.targetPath.is_file():
			error(f"File '{styleError(self.targetPath)}' already exists.")
			return
		self.parentDir = self.targetPath.parent
		self.projectName = self.targetPath.name
		if not self.parentDir.is_dir():
			error(f"Parent directory '{styleError(self.parentDir)}' does not exist.")
			return
		self.sampleProject = getOtterSampleProjectPath()
		if not self.sampleProject.is_dir():
			error(f"OtterSampleProject could not be found at: '{getOtterSampleProjectPath()}'")
			return
		self.valid = True

	def run(self):
		ignores = (
			".git",
			".DS_Store",
			'*.xcuserstate',
		)
		shutil.copytree(
			self.sampleProject,
			self.targetPath,
			ignore=shutil.ignore_patterns(*ignores)
		)
		# rename in file contents
		for root, subdirs, files in os.walk(self.targetPath):
			for file in files:
				filePath = os.path.join(root, file)
				replaceFileContents(filePath, SAMPLE_PROJECT_NAME, self.projectName)

		# rename directories
		dirsToRename = []
		for root, subdirs, files in os.walk(self.targetPath):
			for subdir in subdirs:
				if SAMPLE_PROJECT_NAME in subdir:
					newDirName = subdir.replace(SAMPLE_PROJECT_NAME, self.projectName)
					dirsToRename.append((os.path.join(root, subdir), os.path.join(root, newDirName)))
		for renameDir in dirsToRename:
			os.rename(renameDir[0], renameDir[1])

		# rename files
		for root, subdirs, files in os.walk(self.targetPath):
			for file in files:
				if SAMPLE_PROJECT_NAME in file:
					newFileName = file.replace(SAMPLE_PROJECT_NAME, self.projectName)
					os.rename(os.path.join(root, file), os.path.join(root, newFileName))

		# create git repo
		os.chdir(self.targetPath)
		os.system("git init . > /dev/null")
		os.system("git add . > /dev/null")
		os.system("git commit -m \"initial commit\" > /dev/null")
		success(f"created '{styleSuccess(self.projectName)}' at '{styleSuccess(self.targetPath)}'")
		return True

class Reset(Action):
	def __init__(self, arguments):
		super().__init__()
		if (len(arguments) != 1):
			error("Reset requires argument: " + styleError("<path>"))
			return
		self.targetPath = Path(arguments[0])
		if self.targetPath.is_file():
			error(f"A file exists at '{styleError(self.targetPath)}'. Please select project folder.")
			return
		if not self.targetPath.is_dir():
			error(f"Directory '{styleError(self.targetPath)}' does not exist.")
			return
		self.parentDir = self.targetPath.parent
		self.projectName = self.targetPath.name
		if not self.parentDir.is_dir():
			error(f"Parent directory '{styleError(self.parentDir)}' does not exist.")
			return
		self.sampleProject = getOtterSampleProjectPath()
		if not self.sampleProject.is_dir():
			error(f"OtterSampleProject could not be found at: '{getOtterSampleProjectPath()}'")
			return
		xcodeProjects = []
		for element in self.targetPath.iterdir():
			if element.is_dir() and ".xcodeproj" in element.name:
				xcodeProjects.append(element)
		if len(xcodeProjects) == 0:
			error(f"Could not find any xcode projects at: '{self.targetPath}'.")
			return
		if len(xcodeProjects) > 1:
			projectsString = ",".join(xcodeProjects)
			error(f"Found more than one xcode projects at: '{self.targetPath}'.\n{projectsString}")
			return
		self.xcodeProject = xcodeProjects[0]
		self.valid = True

	def run(self):
		filesToCopy = [
			"project.pbxproj",
		]
		for file in filesToCopy:
			# copy
			sourceAbs = Path(getOtterSampleProjectXcodePath()) / file
			targetAbs = Path(self.xcodeProject) / file
			shutil.copyfile(sourceAbs, targetAbs)
			# rename in file contents
			replaceFileContents(targetAbs.resolve(), SAMPLE_PROJECT_NAME, self.projectName)
		return True




# MAIN
if command == "create":
	action = Create(arguments[1:])
elif command == "reset":
	action = Reset(arguments[1:])
else:
	error("command " + styleError(command) + " not understood")
	abort()

if not action.valid:
	abort()
ranSuccessfully = action.run()
if not ranSuccessfully:
	abort()
