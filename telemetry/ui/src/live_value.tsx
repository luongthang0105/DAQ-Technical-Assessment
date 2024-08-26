import "./App.css";

interface TemperatureProps {
  temp: number;
}

function LiveValue({ temp }: TemperatureProps) {
  const determineColor = () => {
    if (temp < 20 || temp > 80) {
      return "#FF3348";
    } else {
      if ((20 <= temp && temp <= 25) ||
          (75 <= temp && temp <= 80)) {
        return "yellow";
      } else {
        return "#33FF00";
      }
    }
  }
  return (
    <header className="live-value" style={{ color: determineColor() }}>
      {`${temp.toPrecision(3)}°C`}
    </header>
  );
}

export default LiveValue;
