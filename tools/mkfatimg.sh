#!/bin/sh
set -e

echo adding `ls out/bin`...

tools/edimg.exe imgin:tools/fdimg0at.tek \
	copy from:README.md to:@: \
	copy from:h4.txt to:@: \
	copy from:rcS to:@: \
	`ls out/bin | xargs -i -n 1 echo copy from:out/bin/{} to:@:` \
	imgout:hda.img 
