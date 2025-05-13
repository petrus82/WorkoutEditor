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
source=("$pkgname-$pkgver.tar.gz::$url/releases/v$pkgver.tar.gz"
        "$pkgname-$pkgver.tar.gz.asc::$url/releases/v$pkgver.tar.gz.asc")
sha256sums=(''
            'SKIP')

build() {
  cd "$_pkgname-$pkgver"
  mkdir build && cd build
  cmake \
    -S.. \
    -DCMAKE_INSTALL_PREFIX:PATH=/usr \
    -DSHARE_INSTALL_PREFIX:PATH=/usr/share \
  cmake --build . --target all
}

package() {
  cd "$_pkgname-$pkgver/build"
  cmake --install . --prefix="$pkgdir/usr"
}
