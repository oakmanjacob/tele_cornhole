import { createContext, useContext } from "react";

export enum CannonStatus {
    NotConnected = 'NotConnected',
    Disarmed = 'Disarmed',
    Ready = 'Ready'
}

interface ICannonStatusContext {
    cannonStatus: CannonStatus
}

const cannonStatusContextDefaultValues: ICannonStatusContext = {
    cannonStatus: CannonStatus.NotConnected
}

export const CannonStatusContext = createContext<ICannonStatusContext>(cannonStatusContextDefaultValues);

export const useCannonStatus = () => useContext(CannonStatusContext);