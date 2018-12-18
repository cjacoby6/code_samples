

with import <nixpkgs> {};
let deps = with python27Packages;
         [ python
           setuptools
           curses
         ];
    packageName = "communistfarm";
in {
  sdlEnv = stdenv.mkDerivation {
      name = packageName;
      buildInputs = [ stdenv ] ++ deps;
      shellHook =
      let
        red = "\\[\\e[0;31m\\]";
        rcol = "\\[\\e[0m\\]";
        green = "\\[\\e[0;32m\\]";
      in
      ''
        export PROMPT_COMMAND=__prompt_command  # Func to gen PS1 after CMDs

        function __prompt_command() {
            local EXIT="$?"             # This needs to be first
            PS1="(${packageName}) "

            if [ $EXIT != 0 ]; then
                PS1+="${red}[\!] \W$ ${rcol}"
            else
                PS1+="${green}[\!] \W$ ${rcol}"
            fi
        }
      '';
  };
}
