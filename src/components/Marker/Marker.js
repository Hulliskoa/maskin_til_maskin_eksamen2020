import './Marker.css';


const Marker = (props) => {
    const { color, name , onClick} = props;
    return (
        <div>
            <div
                className="pin bounce"
                style={{ backgroundColor: color, cursor: 'pointer' }}
                title={name}
                onClick={onClick}
            />
            <div className="pulse" />
        </div>
    );
};

export default Marker;