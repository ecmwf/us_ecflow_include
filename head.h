set -e # stop the shell on first error
set -u # fail when using an undefined variable
set -x # echo script lines as they are executed

# Defines the variables that are needed for any communication with ECF
export ECF_PORT=%ECF_PORT%    # The server port number
export ECF_HOST=%ECF_HOST%    # The name of ecf host that issued this task
export ECF_NAME=%ECF_NAME%    # The name of this current task
export ECF_PASS=%ECF_PASS%    # A unique password
export ECF_TRYNO=%ECF_TRYNO%  # Current try number of the task
export ECF_RID=$$

module load ecflow/%ECF_VERSION%

# Tell ecFlow we have started
ecflow_client --init=$$

# Defined a error handler
ERROR() {
   set -x
   set +e                      # Clear -e flag, so we don't fail
   wait
   ecflow_client --abort=trap  # Notify ecFlow that something went wrong, using 'trap' as the reason
   trap - 0 $SIGNAL_LIST   # Remove the traps
   echo "The environment was:"
   printenv | sort 
   exit 0                      # End the script
}

# Trap any signal that may cause the script to fail
export SIGNAL_LIST='1 2 3 4 5 6 7 8 10 11 13 15 18 24 31 32 33 34 35 36 37 38 39 40 41 42 43 44 45 46 47 48 49 50 51 52 53 54 55 56 57 58 59 60 61 62 63 64'

for signal in $SIGNAL_LIST
do
  name=$(kill -l $signal)
  trap "{ echo \"Signal $name ($signal) received \"; trap - 0 $SIGNAL_LIST ; ERROR ; }" $signal
done

# Trap any calls to exit and errors caught by the -e flag
trap "{ echo \"Signal EXIT (0) received \"; trap - 0 $SIGNAL_LIST ; ERROR $signal ; }" 0
trap 
