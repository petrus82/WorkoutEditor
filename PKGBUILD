pkgname=workoutchart
_pkgname=WorkoutChart
pkgver=0.0.1
pkgrel=1
pkgdesc="Application to create cycling workout files in .fit, .plan, .erg or .mrg format"
url="https://github.com/petrus82/WorkoutEditor"
license=('GPL3')
arch=('i686' 'x86_64')
depends=('qt6-base' 'qt6-declarative')
makedepens=('cmake')
provides=(workoutchart)
source=("workoutchart::git+$url")
sha256sums=('SKIP')

build() {
  echo $PWD
  cd WorkoutEditor-$pkgver
  mkdir -p build && cd build
  cmake .. \
    -DCMAKE_INSTALL_PREFIX:PATH=/usr \
    -DSHARE_INSTALL_PREFIX:PATH=/usr/share
  cmake --build . --target all
}

package() {
  cd WorkoutEditor-$pkgver/build
  cmake --install . --prefix="$pkgdir/usr"
}
