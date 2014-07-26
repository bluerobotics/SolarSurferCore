REPO=SolarSurferMessage

# checkout repo
if [ ! -d "$REPO" ]; then
  git clone https://github.com/bluerobotics/SolarSurferMessage.git $REPO
fi

# update repo
cd $REPO
git pull origin master

# install and cross-compile C lib
npm install
npm run-script makeclib

# move to src
mv output/Messages.h ../src/
