import axios from "axios"
import { useEffect, useState } from "react"
import { CANNON_URL } from "../constants"
import { CannonStatus } from "./cannon-status-context"

export const usePollCannonStatus = () => {
    const [ cannonStatus, setCannonStatus ] = useState<CannonStatus>(CannonStatus.NotConnected)

    const pollCurrentStatus = () => {
        axios.get(`${CANNON_URL}/status`).then((response) => {
            switch(response.data) {
                case 'Ready':
                    setCannonStatus(CannonStatus.Ready);
                    break;
                case 'Disarmed':
                    setCannonStatus(CannonStatus.Disarmed);
                    break;
                case 'NotConnected':
                default:
                    setCannonStatus(CannonStatus.NotConnected);
                    break;
            }
        })
    }

    useEffect(() => {
        const pollInterval = setInterval(pollCurrentStatus);
        return () => {
            clearInterval(pollInterval);
        }
    }, [])

    return cannonStatus;
}