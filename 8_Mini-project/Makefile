include $(TOPDIR)/rules.mk

PKG_NAME:=speedtest-cli
PKG_VERSION:=1.0
PKG_RELEASE:=2

PKG_BUILD_DIR := $(BUILD_DIR)/$(PKG_NAME)


include $(INCLUDE_DIR)/package.mk

define Package/speedtest-cli
	SECTION:=net
	CATEGORY:=Network
	TITLE:=speedtest client
	DEPENDS:=+libpthread
endef

define Package/speedtest-cli/description
	speedtest-cli is the client tool (written in C) to measure upload and download throughput 
	of a computer against nearest speedtest.net public server.
endef

TARGET_CFLAGS += -ggdb3

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef


define Package/speedtest-cli/install
	$(INSTALL_DIR) $(1)/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/speedtest-cli $(1)/bin/
endef


$(eval $(call BuildPackage,speedtest-cli))
