{ pkgs ? import <nixpkgs> { } }:
pkgs.mkShell {
  buildInputs = with pkgs; [
    dfu-programmer
    pkgsCross.avr.buildPackages.gcc
    pkgsCross.avr.buildPackages.binutils
  ];
}
