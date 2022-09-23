import './Control.css';

import TargetImage from './assets/target.jpg';
import { ControlOverlay } from '../../components/control-overlay/control-overlay';

const Control: React.FC = () => {

    return (
        <div className="control-container">
            <ControlOverlay />
            <img className="target-image" src={TargetImage} alt="" />
        </div>
    )
}

export default Control