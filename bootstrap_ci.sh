#!/bin/bash
# Use ci3 script base.
source $(git rev-parse --show-toplevel)/ci3/base/source

NO_TERMINATE=${NO_TERMINATE:-0}

function on_exit {
    set +e
    if [ -n "${ip:-}" ] && [ "$NO_TERMINATE" -eq 0 ]; then
        echo "Terminating spot instance..."
        ssh ubuntu@$ip sudo halt -p > /dev/null 2>&1
        aws ec2 cancel-spot-instance-requests --spot-instance-request-ids $sir >/dev/null 2>&1 || true
    fi
    if [ -n "${ip:-}" ] && [ "$NO_TERMINATE" -ne 0 ]; then
      echo "Remote machine not terminated, connect with: ssh -t ubuntu@$ip 'docker start aztec_build >/dev/null 2>&1 || true && docker exec -it aztec_build bash'"
    fi
}
trap on_exit EXIT

cd $(dirname $0)

current_commit=$(git rev-parse HEAD)

# Verify that the commit exists on the remote. It will be the remote tip of itself if so.
if [[ "$(git fetch origin --negotiate-only --negotiation-tip=$current_commit)" != *"$current_commit"* ]] ; then
  echo "Commit $current_commit is not pushed, exiting."
  exit 1
fi

$ci3/github/group "Request Build Instance"
ip_sir=$($ci3/aws/request_instance ci3-$USER 128 x86_64)
parts=(${ip_sir//:/ })
ip="${parts[0]}"
sir="${parts[1]}"
$ci3/github/endgroup

GITHUB_LOG=${GITHUB_ACTIONS:-}
# pass env vars to inform if we are inside github actions, and our AWS creds
args="-e GITHUB_LOG='$GITHUB_LOG' -e AWS_ACCESS_KEY_ID='${AWS_ACCESS_KEY_ID:-}' -e AWS_SECRET_ACCESS_KEY='${AWS_SECRET_ACCESS_KEY:-}'"
[ "$NO_TERMINATE" -eq 0 ] && args+=" --rm"

$ci3/github/group "Start CI Image"

# - Use ~/.ssh/build_instance_key to ssh into our requested instance (note, could be on-demand if spot fails)
# - Run in our build container, cloning commit and running bootstrap.sh
ssh -F $ci3/aws/build_instance_ssh_config ubuntu@$ip "
  docker run --privileged $args --name aztec_build -t \
    -v boostrap_ci_local_docker:/var/lib/docker \
    aztecprotocol/ci:2.0 bash -c '
      [ -n \"${GITHUB_LOG:-}\" ] && echo "::endgroup::"
      [ -n \"${GITHUB_LOG:-}\" ] && echo "::group::Clone Repository"
      set -e
      # When restarting the container, just hang around.
      while [ -f started ]; do sleep 999; done
      touch started
      /usr/local/share/docker-init.sh &> /dev/null
      mkdir -p /root/aztec-packages
      cd /root/aztec-packages
      git init &>/dev/null
      git remote add origin http://github.com/aztecprotocol/aztec-packages
      git fetch --depth 1 origin $current_commit
      git checkout FETCH_HEAD >/dev/null
      [ -n \"${GITHUB_LOG:-}\" ] && echo "::endgroup::"
      CI=0 ./bootstrap.sh fast
    '
"