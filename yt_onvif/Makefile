#
# Copyright (C) 2006 OpenWrt.org
#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=yt_onvif
PKG_VERSION:=1.0.1
PKG_RELEASE:=1


include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/yt_onvif
  SECTION:=net
  CATEGORY:=Xinzailing
  DEPENDS:=+libopenssl  +zlib 
  TITLE:=yt_onvif utility
endef

define Package/yt_onvif/description
	This package is usocket utility
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef



define Package/yt_onvif/conffiles

endef



define Package/yt_onvif/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/sbin/yt_onvif $(1)/usr/sbin/
endef

$(eval $(call BuildPackage,yt_onvif))
