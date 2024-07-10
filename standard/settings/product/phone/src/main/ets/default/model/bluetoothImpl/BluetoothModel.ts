/**
 * Copyright (c) 2021 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
import BaseModel from '../../../../../../../../common/utils/src/main/ets/default/model/BaseModel';
import LogUtil from '../../../../../../../../common/utils/src/main/ets/default/baseUtil/LogUtil';
import ConfigData from '../../../../../../../../common/utils/src/main/ets/default/baseUtil/ConfigData';
import {LogAll} from '../../../../../../../../common/utils/src/main/ets/default/baseUtil/LogDecorator';
import bluetooth from '@ohos.bluetooth';

export enum ProfileCode {
  CODE_BT_PROFILE_A2DP_SINK = 0,
  CODE_BT_PROFILE_A2DP_SOURCE,
  CODE_BT_PROFILE_AVRCP_CT,
  CODE_BT_PROFILE_AVRCP_TG,
  CODE_BT_PROFILE_HANDS_FREE_AUDIO_GATEWAY,
  CODE_BT_PROFILE_HANDS_FREE_UNIT,
  CODE_BT_PROFILE_HID_HOST,
  CODE_BT_PROFILE_PAN_NETWORK,
  CODE_BT_PROFILE_PBAP_CLIENT,
  CODE_BT_PROFILE_PBAP_SERVER,
};

export enum BondState {
  /** Indicate the bond state is invalid */
  BOND_STATE_INVALID = 0,
  /** Indicate the bond state is bonding */
  BOND_STATE_BONDING = 1,
  /** Indicate the bond state is bonded*/
  BOND_STATE_BONDED = 2
}

export enum DeviceType {
  BLUETOOTH = '1',
  HEADPHONE = '2',
  PHONE = '3',
  COMPUTER = '4',
  WATCH = '5'
}

export enum DeviceState {
  /** the device is disconnected */
  STATE_DISCONNECTED = 0,
  /** the device is being connected */
  STATE_CONNECTING = 1,
  /** the device is connected */
  STATE_CONNECTED = 2,
  /** the device is being disconnected */
  STATE_DISCONNECTING = 3,
  /** the device is available */
  STATE_AVAILABLE = 100,
  /** the device is pairing */
  STATE_PAIRING = 101,
  /** the device is paired */
  STATE_PAIRED = 102
}

export enum BluetoothErrorCode {
  SUCCESS = -1,
  HOLD_PAIRING_MODE = 1,
  APP_PAIR = 2,
  PAIR_FAILED = 3,
  DEVICE_ILLEGAL = 4,
  CONNECT_FAILED = 5
}

enum BluetoothState {
  /** Indicates the local Bluetooth is off */
  STATE_OFF = 0,
  /** Indicates the local Bluetooth is turning on */
  STATE_TURNING_ON = 1,
  /** Indicates the local Bluetooth is on, and ready for use */
  STATE_ON = 2,
  /** Indicates the local Bluetooth is turning off */
  STATE_TURNING_OFF = 3,
  /** Indicates the local Bluetooth is turning LE mode on */
  STATE_BLE_TURNING_ON = 4,
  /** Indicates the local Bluetooth is in LE only mode */
  STATE_BLE_ON = 5,
  /** Indicates the local Bluetooth is turning off LE only mode */
  STATE_BLE_TURNING_OFF = 6
}

/**
 * bluetooth service class
 */
@LogAll
export class BluetoothModel extends BaseModel {
  private TAG = ConfigData.TAG + 'BluetoothModel ';
  private profiles: any[] = new Array(10);
  public canUse: boolean = false;

  /**
   * constructor
   */
  constructor() {
    super();
    try{
      this.profiles[ProfileCode.CODE_BT_PROFILE_A2DP_SOURCE] = bluetooth.getProfile(ProfileCode.CODE_BT_PROFILE_A2DP_SOURCE);
      this.profiles[ProfileCode.CODE_BT_PROFILE_HANDS_FREE_AUDIO_GATEWAY] = bluetooth.getProfile(ProfileCode.CODE_BT_PROFILE_HANDS_FREE_AUDIO_GATEWAY);
      this.profiles[ProfileCode.CODE_BT_PROFILE_HID_HOST] = bluetooth.getProfile(ProfileCode.CODE_BT_PROFILE_HID_HOST);
      this.canUse = true;
      }
    catch(error){
      this.canUse = false;
      LogUtil.info(`BluetoothModel error: ${JSON.stringify(error)}.`);
    }
  }


  /**
   * Get Bluetooth status
   * @return value of bluetooth.BluetoothState type
   */
  getState(): number {
    let bluetoothState = bluetooth.getState();
    LogUtil.info(`${this.TAG} getState: bluetoothState = ${bluetoothState}`);
    return bluetoothState;
  }

  /**
   * Get Bluetooth switch status
   */
  isStateOn(): boolean {
    let result = false;
    let state = bluetooth.getState();
    LogUtil.info(`${this.TAG} isStateOn: state = ${state}`);
    switch (state) {
      case BluetoothState.STATE_ON:
        result = true
        break;
      default:
        break;
    }
    LogUtil.info(`${this.TAG} isStateOn: bluetoothState = ${result}`);
    return result;
  }

  /**
   * Subscribe Bluetooth switch status Change
   */
  subscribeStateChange(callback: (data: boolean) => void): void {
    bluetooth.on('stateChange', (data) => {
      LogUtil.info(`${this.TAG} subscribeStateChange->stateChange data:${data}`);
      if (callback) {
        switch (data) {
          case BluetoothState.STATE_ON:
            bluetooth.setBluetoothScanMode(4, 0);
            LogUtil.info(`${this.TAG} subscribeStateChange->stateChange return: true`);
            callback(true)
            break;

          case BluetoothState.STATE_OFF:
            LogUtil.info(`${this.TAG} subscribeStateChange->stateChange return: false`);
            callback(false)
            break;

          default:
            break;
        }
      }
    })
  }

  /**
   * unsubscribe Bluetooth switch status Change
   */
  unsubscribeStateChange(callback?: (data: boolean) => void): void {
    bluetooth.off('stateChange', (data) => {
      LogUtil.info(`${this.TAG} unsubscribeStateChange->stateChange data:${data}`);
      if (callback) {
        let result = false;
        switch (data) {
          case BluetoothState.STATE_ON:
            LogUtil.info(`${this.TAG} unsubscribeStateChange->stateChange return : true`);
            callback(true)
            break;
          case BluetoothState.STATE_OFF:
            LogUtil.info(`${this.TAG} unsubscribeStateChange->stateChange return : false`);
            callback(false)
            break;
          default:
            break;
        }
      }
    })
  }

  /**
   * Turn on Bluetooth
   */
  enableBluetooth(): boolean {
    return bluetooth.enableBluetooth();
  }

  /**
   * Turn off Bluetooth
   */
  disableBluetooth(): boolean {
    return bluetooth.disableBluetooth();
  }

  /**
   * Get local name
   */
  getLocalName(): string {
    return bluetooth.getLocalName();
  }

  /**
   * Set local name
   */
  setLocalName(name: string): boolean {
    return bluetooth.setLocalName(name);
  }

  /**
   * Get paired device ids
   */
  getPairedDeviceIds(): Array<string> {
    return bluetooth.getPairedDevices();
  }

  /**
   * Start Bluetooth discovery
   */
  startBluetoothDiscovery(): boolean {
    return bluetooth.startBluetoothDiscovery();
  }

  /**
   * Stop Bluetooth discovery
   */
  stopBluetoothDiscovery(): boolean {
    return bluetooth.stopBluetoothDiscovery();
  }

  /**
   * Subscribe Bluetooth status Change
   */
  subscribeBluetoothDeviceFind(callback: (data: Array<string>) => void): void {
    bluetooth.on('bluetoothDeviceFind', (data: Array<string>) => {
      LogUtil.info(`${this.TAG} subscribeBluetoothDeviceFind->deviceFind return:${JSON.stringify(data)}`);
      if (callback) {
        callback(data)
      }
    })
  }

  /**
   * unsubscribe Bluetooth status Change
   */
  unsubscribeBluetoothDeviceFind(callback?: (data: Array<string>) => void): void {
    bluetooth.off('bluetoothDeviceFind', (data) => {
      LogUtil.info(`${this.TAG} unsubscribeBluetoothDeviceFind->deviceFind return:${JSON.stringify(data)}`);
      if (callback) {
        callback(data)
      }
    })
  }

  /**
   * Pair device
   */
  pairDevice(deviceId: string): boolean {
    return bluetooth.pairDevice(deviceId);
  }

  /**
   * Subscribe PinRequired
   */
  subscribePinRequired(callback: (data: {
    deviceId: string;
    pinCode: string;
  }) => void): void {
    bluetooth.on('pinRequired', (data: {
      deviceId: string;
      pinCode: string;
    }) => {
      LogUtil.info(`${this.TAG} subscribePinRequired->pinRequired return:${JSON.stringify(data)}`);
      if (callback) {
        callback(data)
      }
    })
  }

  /**
   * Unsubscribe PinRequired
   */
  unsubscribePinRequired(callback?: (data: {
    deviceId: string;
    pinCode: string;
  }) => void): void {
    bluetooth.off('pinRequired', (data: {
      deviceId: string;
      pinCode: string;
    }) => {
      LogUtil.info(`${this.TAG} unsubscribePinRequired->pinRequired return: ${JSON.stringify(data)}`);
      if (callback) {
        callback(data)
      }
    })
  }

  /**
   * Set device PairingConfirmation
   */
  setDevicePairingConfirmation(deviceId: string, accept: boolean): boolean {
    return bluetooth.setDevicePairingConfirmation(deviceId, accept);
  }

  /**
   * Subscribe bondStateChange
   */
  subscribeBondStateChange(callback): void {
    bluetooth.on('bondStateChange', (data) => {
      LogUtil.info(`${this.TAG} subscribeBondStateChange->bondStateChange data:${JSON.stringify(data)}`);
      if (callback) {
        let result = {
          deviceId: data.deviceId,
          bondState: data.state
        }
        LogUtil.info(`${this.TAG} subscribeBondStateChange->bondStateChange return:${JSON.stringify(result)}`);
        callback(result);
      }
    })
  }

  /**
   * Unsubscribe bondStateChange
   */
  unsubscribeBondStateChange(callback?: (data: {
    deviceId: string;
    bondState: number;
  }) => void): void {
    bluetooth.off('bondStateChange', (data) => {
      LogUtil.info(`${this.TAG} unsubscribeBondStateChange->bondStateChange data:${JSON.stringify(data)}`);
      if (callback) {
        let result = {
          deviceId: data.deviceId,
          bondState: data.state
        }
        LogUtil.info(`${this.TAG} unsubscribeBondStateChange->bondStateChange return:${JSON.stringify(result)}`);
        callback(result);
      }
    })
  }

  /**
   * Get device name
   */
  getDeviceName(deviceId: string): string {
    return bluetooth.getRemoteDeviceName(deviceId);
  }

  /**
   * Get device type
   */
  getDeviceType(deviceId: string): string {
    let deviceType = DeviceType.BLUETOOTH;
    let deviceClass = bluetooth.getRemoteDeviceClass(deviceId);
    switch (deviceClass.majorClass) {
      case 0x0100:
        deviceType = DeviceType.COMPUTER;
        break;
      case 0x0400:
        if (deviceClass.majorMinorClass === 0x0418 || deviceClass.majorMinorClass === 0x0404) {
          deviceType = DeviceType.HEADPHONE;
        }
        break;
      case 0x0700:
        if (deviceClass.majorMinorClass === 0x0704) {
          deviceType = DeviceType.WATCH;
        }
        break;
      case 0x0200:
        deviceType = DeviceType.PHONE;
        break;
      default:
        deviceType = DeviceType.BLUETOOTH;
        break;
    }
    return deviceType;
  }

  /**
   * Get device state
   */
  getDeviceState(deviceId: string): Array<{
    profileId: number;
    profileConnectionState: number;
  }> {
    let result = [];
    for (let i = 0;i < this.profiles.length; i++) {
      if (this.profiles[i]) {
        let state = this.profiles[i].getDeviceState(deviceId);
        result.push({
          profileId: i,
          profileConnectionState: state
        });
      }
    }
    return result;
  }

  /**
   * Unpair device
   */
  unpairDevice(deviceId: string): boolean {
    return bluetooth.cancelPairedDevice(deviceId);
  }

  /**
   * Connect device
   */
  connectDevice(deviceId: string): Array<{
    profileId: number;
    connectRet: boolean;
  }> {
    let result = [];
    for (let i = 0;i < this.profiles.length; i++) {
      if (this.profiles[i]) {
        let profile = this.profiles[i];
        let connectRet = profile.connect(deviceId);
        result.push({
          profileId: i,
          connectRet: connectRet
        });
      }
    }
    return result;
  }

  /**
   * Disconnect device
   */
  disconnectDevice(deviceId: string): Array<{
    profileId: number;
    disconnectRet: boolean;
  }> {
    let result = [];
    for (let i = 0;i < this.profiles.length; i++) {
      let profile = this.profiles[i];
      if (this.profiles[i]) {
        let profileConnectionState = profile.getDeviceState(deviceId);
        let disconnectRet = true;
        LogUtil.info(`${this.TAG} disconnectDevice deviceId = ${deviceId}, connectionState = ${profileConnectionState}`);
        if (profileConnectionState === 2) {
        disconnectRet = profile.disconnect(deviceId);
        LogUtil.info(`${this.TAG} disconnectDevice call disconnect over. api return =  ${disconnectRet}, deviceId = ${deviceId}`);
        }
        result.push({
          profileId: i,
          disconnectRet: disconnectRet
        });
      }
    }
    return result;
  }

  /**
   * Subscribe device connection state Change
   */
  subscribeDeviceStateChange(callback: (data: {
    profileId: number;
    deviceId: string;
    profileConnectionState: number;
  }) => void): void {
    for (let i = 0;i < this.profiles.length; i++) {
      if (this.profiles[i]) {
        let profile = this.profiles[i];
        profile.on('connectionStateChange', (data) => {
          LogUtil.info(`${this.TAG} subscribeDeviceStateChange->connectionStateChange data:${JSON.stringify(data)}`);
          if (callback) {
            let result = {
              profileId: i,
              deviceId: data.deviceId,
              profileConnectionState: data.state
            };
            LogUtil.info(`${this.TAG} subscribeDeviceStateChange->connectionStateChange return:${JSON.stringify(result)}`);
            callback(result);
          }
        })
      }
    }
  }

  /**
   * unsubscribe device connection state Change
   */
  unsubscribeDeviceStateChange(callback?: (data: {
    profileId: number;
    deviceId: string;
    profileConnectionState: number;
  }) => void): void {
    for (let i = 0;i < this.profiles.length; i++) {
      if (this.profiles[i]) {
        let profile = this.profiles[i];
        profile.off('connectionStateChange', (data) => {
          LogUtil.info(`${this.TAG} unsubscribeDeviceStateChange->connectionStateChange data:${JSON.stringify(data)}`);
          if (callback) {
            let result = {
              profileId: i,
              deviceId: data.deviceId,
              profileConnectionState: data.state
            };
            LogUtil.info(`${this.TAG} unsubscribeDeviceStateChange->connectionStateChange return:${JSON.stringify(result)}`);
            callback(result);
          }
        })
      }
    }
  }
}

let bluetoothModel = new BluetoothModel();

export default bluetoothModel as BluetoothModel;