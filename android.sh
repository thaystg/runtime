set -e

NDK_VER=r23c
SDK_VER=9123335_latest
SDK_API_LEVEL=33
SDK_BUILD_TOOLS=33.0.1

if [[ "$OSTYPE" == "darwin"* ]]; then
    HOST_OS=darwin
    HOST_OS_SHORT=mac
    BASHRC=~/.zprofile
else
    HOST_OS=linux
    HOST_OS_SHORT=linux
    BASHRC=~/.bashrc
fi

# download Android NDK
export ANDROID_NDK_ROOT=~/android-ndk-${NDK_VER}
curl https://dl.google.com/android/repository/android-ndk-${NDK_VER}-${HOST_OS}.zip -L --output ~/andk.zip
unzip ~/andk.zip -d $(dirname ${ANDROID_NDK_ROOT}) && rm -rf ~/andk.zip

# download Android SDK, accept licenses and download additional packages such as
# platform-tools, platforms and build-tools
export ANDROID_SDK_ROOT=~/android-sdk
curl https://dl.google.com/android/repository/commandlinetools-${HOST_OS_SHORT}-${SDK_VER}.zip -L --output ~/asdk.zip
mkdir ${ANDROID_SDK_ROOT} && unzip ~/asdk.zip -d ${ANDROID_SDK_ROOT}/cmdline-tools && rm -rf ~/asdk.zip
yes | ${ANDROID_SDK_ROOT}/cmdline-tools/cmdline-tools/bin/sdkmanager --sdk_root=${ANDROID_SDK_ROOT} --licenses
${ANDROID_SDK_ROOT}/cmdline-tools/cmdline-tools/bin/sdkmanager --sdk_root=${ANDROID_SDK_ROOT} "platform-tools" "platforms;android-${SDK_API_LEVEL}" "build-tools;${SDK_BUILD_TOOLS}"
