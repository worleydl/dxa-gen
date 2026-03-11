A small collection of utilities to generate your own custom Descent dxa soundtrack.

## headaches
- zita resampler may be fussy about includes
- plays nicest under mingw
- The cmake application and associated bash script requires the original Descent hmp/hmq/hmqq files, you can extract them with the python utility under `extractor`, just place `descent.hog` next to the script.
- The bash script looks for `hmq_dump` next to itself and currently requires copying the output from extractor
- You will still need to zip up the final dxa yourself, maybe provide descent.sng too? not sure if that's required...

---

GPT was used to scaffold parts of this project.
Licensed under The Unlicense
