## From forum post on accessing .env file for git ignoring protected information.
## Geißler, F., &amp; resse. (2020, March 1). Access of outer environment variable in Platformio. Stack Overflow. Retrieved April 22, 2023, from https://stackoverflow.com/questions/62314497/access-of-outer-environment-variable-in-platformio 


from os.path import isfile
Import("env")
assert isfile(".env")
try:
  f = open(".env", "r")
  lines = f.readlines()
  envs = []
  for line in lines:
    envs.append("-D{}".format(line.strip()))
  env.Append(BUILD_FLAGS=envs)
except IOError:
  print("File .env not accessible",)
finally:
  f.close()