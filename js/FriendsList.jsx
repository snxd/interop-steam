import {useEffect, useRef} from "react";

import {interop} from "../direct/Host";

import {steamFriends, steamFriendsPersonaState} from "./SteamFriends";
import {steamUtils} from "./SteamUtils";

const convertBase64ToBinary = (base64) => {
  const raw = window.atob(base64);
  let array = new Uint8ClampedArray(new ArrayBuffer(raw.length));
  for (let i = 0; i < raw.length; i++) {
    array[i] = raw.charCodeAt(i);
  }
  return array;
};

export default function FriendsList() {
  const canvasRef = useRef(null);
  const loadedRef = useRef(false);

  const drawList = (canvas) => {
    const ctx = canvas.getContext("2d");

    const friendCount = steamFriends.getFriendCount();

    // Draw friend list to canvas element
    for (let i = 0, y = 0; i < friendCount; i += 1) {
      const friendSteamId = steamFriends.getFriendByIndex(i);
      const friendPersonaName = steamFriends.getFriendPersonaName(friendSteamId);
      const friendPersonaState = steamFriends.getFriendPersonaState(friendSteamId);
      const friendPersonaStateString = steamFriendsPersonaState.nameFromId(friendPersonaState);

      const friendAvatarIndex = steamFriends.getSmallFriendAvatar(friendSteamId);
      const friendAvatarWidth = steamUtils.getImageWidth(friendAvatarIndex);
      const friendAvatarHeight = steamUtils.getImageHeight(friendAvatarIndex);

      console.log(`SteamUtils - Image ${friendPersonaName} - ${friendAvatarIndex} - ${friendAvatarWidth}x${friendAvatarHeight}`);

      const friendAvatarRGBA = steamUtils.getImageRGBA(friendAvatarIndex);
      const friendAvatarImageArray = convertBase64ToBinary(friendAvatarRGBA);
      const friendAvatarImageData = new ImageData(friendAvatarImageArray, friendAvatarWidth, friendAvatarHeight);

      ctx.putImageData(friendAvatarImageData, 0, y);
      ctx.fillStyle = "white";
      ctx.fillText(`${friendPersonaName} (${friendPersonaStateString})`, friendAvatarWidth + 10, y + ((friendAvatarHeight / 2) + 10));

      y += friendAvatarHeight;
    }
  };

  useEffect(() => {
    interop.on("load", (info) => {
      if (info.name === "steam") {
        loadedRef.current = true;
      }
    });
    // eslint-disable-next-line
  }, []);

  useEffect(() => {
    drawList(canvasRef.current);
  }, [loadedRef]);

  return (<canvas ref={canvasRef} width="300" height="600"></canvas>);
}
